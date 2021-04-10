#include "luapatcher.h"
#include "lua.h"
#include "util.h"
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#define LUAPATCHER_INST "_luapatcher_inst"

template <class T>
CPortableExecutable* LuaPatcher_pe_Tmember(lua_State* L, T** out_Data);
int minilua_checkargs(lua_State* L, const char* Fmt, ...);

struct FuncPair
{
	const char* name;
	lua_CFunction func;
};

static const FuncPair pe_api_funcs[] = {
	{"_new",		&CLuaPatcher::_pe_new},
	{"_addr",		&CLuaPatcher::_pe_addr},
	{"_len",		&CLuaPatcher::_pe_len},
	{"_baseaddr",	&CLuaPatcher::_pe_base},
	{"_codeaddr",	&CLuaPatcher::_pe_codeaddr},
	{"_codelen",	&CLuaPatcher::_pe_codelen},
	{"_entry",		&CLuaPatcher::_pe_entry},
	{"_nexps",		&CLuaPatcher::_pe_nexps},
	{"_exp",		&CLuaPatcher::_pe_exp},
	{"_patch",		&CLuaPatcher::_pe_patch},
	{"_patchhex",	&CLuaPatcher::_pe_patchhex},
	{"_save",		&CLuaPatcher::_pe_save},
	{NULL, NULL}
};

static const FuncPair ct_api_funcs[] = {
	{"_new",		&CLuaPatcher::_ct_new},
	{"_ndatas",		&CLuaPatcher::_ct_ndatas},
	{"_data",		&CLuaPatcher::_ct_data},
	{"_nfuncs",		&CLuaPatcher::_ct_nfuncs},
	{"_func",		&CLuaPatcher::_ct_func},
	{"_findfunc",	&CLuaPatcher::_ct_findfunc},
	{"_strref",		&CLuaPatcher::_ct_strref},
	{"_addpage",	&CLuaPatcher::_ct_addpage},
	{"_traverse",	&CLuaPatcher::_ct_traverse},
	{"_readstr",	&CLuaPatcher::_ct_readstr},
	{NULL, NULL}
};

static const FuncPair dialog_api_funcs[] = {
	{"_file",	&CLuaPatcher::_dialog_file},
	{"_msgbox", &CLuaPatcher::_dialog_msgbox},
	{NULL, NULL}
};

static const std::pair<const char*, const FuncPair*> apis[] =
{
	{ "pe_api",		pe_api_funcs },
	{ "ct_api",		ct_api_funcs },
	{ "dialog_api", dialog_api_funcs },
};

CLuaPatcher::CLuaPatcher(lua_State* L) : m_state(L)
{
	lua_pushlightuserdata(L, this);
	lua_setglobal(L, LUAPATCHER_INST);

	for (const auto& api : apis)
	{
		lua_newtable(L);

		for (const FuncPair* pair = api.second; pair->func; ++pair)
		{
			lua_pushcfunction(L, pair->func);
			lua_setfield(L, -2, pair->name);
		}

		lua_setglobal(L, api.first);
	}
}

CLuaPatcher::~CLuaPatcher()
{
	for (CLuaManagedPtrBase* ptr : m_managed)
		delete ptr;
}

int CLuaPatcher::_pe_new(lua_State* L)
{
	const char*	fname;
	int			maxsize;

	size_t	flen;
	char*	fdata;
	CPortableExecutable* pe;
	void**	usrdata;

	if (!minilua_checkargs(L, "si", &fname, &maxsize) || maxsize <= 0)
		return fprintf(stderr, "_pe_new: Bad arguments\n"), 0;

	if (!(fdata = Util::GetFileData(fname, &flen)))
		return fprintf(stderr, "_pe_new: Failed to read file '%s'\n", fname), 0;

	pe = new CPortableExecutable(fdata, flen, maxsize);

	if (!pe->IsValid())
		return delete pe, 0;

	CLuaPatcher::PushNewInstance(L, pe);
	return 1;
}

int CLuaPatcher::_pe_addr(lua_State* L)
{
	uint64_t* usrdata;
	if (CPortableExecutable* pe = LuaPatcher_pe_Tmember<uint64_t>(L, &usrdata))
		return *usrdata = (uint64_t)pe->ImgData(), 1;
	return 0;
}

int CLuaPatcher::_pe_len(lua_State* L)
{
	uint64_t* usrdata;
	if (CPortableExecutable* pe = LuaPatcher_pe_Tmember<uint64_t>(L, &usrdata))
		return *usrdata = (uint64_t)pe->ImgLen(), 1;
	return 0;
}

int CLuaPatcher::_pe_base(lua_State* L)
{
	uint64_t* usrdata;
	if (CPortableExecutable* pe = LuaPatcher_pe_Tmember<uint64_t>(L, &usrdata))
		return *usrdata = pe->ImageBase(), 1;
	return 0;
}

int CLuaPatcher::_pe_codeaddr(lua_State* L)
{
	uint64_t* usrdata;
	if (CPortableExecutable* pe = LuaPatcher_pe_Tmember<uint64_t>(L, &usrdata))
		return *usrdata = (uint64_t)pe->ImgData() + pe->OptionalHeader().BaseOfCode(), 1;
	return 0;
}

int CLuaPatcher::_pe_codelen(lua_State* L)
{
	uint64_t* usrdata;
	if (CPortableExecutable* pe = LuaPatcher_pe_Tmember<uint64_t>(L, &usrdata))
		return *usrdata = pe->OptionalHeader().SizeOfCode(), 1;
	return 0;
}

int CLuaPatcher::_pe_entry(lua_State* L)
{
	uint64_t* usrdata;
	if (CPortableExecutable* pe = LuaPatcher_pe_Tmember<uint64_t>(L, &usrdata))
		return *usrdata = (uint64_t)pe->CodeEntry(), 1;
	return 0;
}

int CLuaPatcher::_pe_nexps(lua_State* L)
{
	CPortableExecutable* pe;
	if (!minilua_checkargs(L, "l", &pe) || !pe->IsValid())
		return fprintf(stderr, "_pe_nexps: Bad arguments\n"), 0;

	lua_pushinteger(L, pe->Exports().size());
	return 1;
}

int CLuaPatcher::_pe_exp(lua_State* L)
{
	CPortableExecutable* pe;
	const PE_LocName* exp = 0;
	size_t i;
	int index;

	if (!minilua_checkargs(L, "li", &pe, &index) ||
		!pe->IsValid() || index < 1 || index > pe->Exports().size())
		return fprintf(stderr, "_pe_exp: Bad arguments\n"), 0;

	i = 0;
	index -= 1; // LUA indexes start at 1
	for (auto it = pe->Exports().cbegin(); it != pe->Exports().cend() && i <= index; ++it, ++i)
	{
		if (i == index)
			exp = &(*it);
	}

	lua_createtable(L, 0, 3);

	lua_pushstring(L, exp->name);
	lua_setfield(L, -2, "name");

	lua_pushinteger(L, exp->ordinal);
	lua_setfield(L, -2, "ordinal");

	PushAddress(L, exp->loc);
	lua_setfield(L, -2, "loc");

	return 1;
}

int CLuaPatcher::_pe_patchhex(lua_State* L)
{
	CPortableExecutable* pe;
	uint64_t* addr;
	int len = 1;
	const char* str;
	char* pos;

	if (!minilua_checkargs(L, "lus", &pe, &addr, &str) ||
		len <= 0 ||
		!pe->IsInBounds(*(void**)addr, len))
		return fprintf(stderr, "_pe_patchhex: Bad arguments\n"), 0;

	pos = *(char**)addr;

	for (int i = 0; str[i]; ++i)
	{
		while (str[i] == ' ')
			++i;
		if (!str[i])
			break;

		if (str[i] == '?')
		{
			i += str[++i] == '?';
			++pos;
		}
		else if (HEXCHAR_ISHEX(str[i]) && HEXCHAR_ISHEX(str[i + 1]))
		{
			if (!pe->IsInBounds(pos, 1))
				return fprintf(stderr, "_pe_patchhex: Patch bytes went out of PE bounds\n"), 0;
			
			*pos = (HEXCHAR_TOHEX(str[i]) << 4) | HEXCHAR_TOHEX(str[i + 1]);
			++i;
			++pos;
		}
		else
			return fprintf(stderr, "_pe_patchhex: Bad format '%s' (char %d)\n", str, i), 0;
	}

	return 0;
}

int CLuaPatcher::_pe_patch(lua_State* L)
{
	CPortableExecutable* pe;
	uint64_t* addr;
	size_t len = 1;
	const char* str;
	char* pos;

	if (!minilua_checkargs(L, "lus", &pe, &addr, &str) ||
		len <= 0 ||
		!pe->IsInBounds(*(void**)addr, len))
		return fprintf(stderr, "_pe_patch: Bad arguments\n"), 0;

	pos = *(char**)addr;

	lua_tolstring(L, -1, &len);

	if (!pe->IsInBounds(pos + len, 1))
		return fprintf(stderr, "_pe_patch: Patch bytes went out of PE bounds\n"), 0;
	
	memcpy(pos, str, len);

	return 0;
}

int CLuaPatcher::_pe_save(lua_State* L)
{
	CPortableExecutable* pe;
	const char* fname;
	FILE* file;

	if (!minilua_checkargs(L, "ls", &pe, &fname) || !pe->IsValid())
		return fprintf(stderr, "_pe_save: Bad arguments\n"), 0;

	pe->WriteMappedToFile();

	if (errno_t err = fopen_s(&file, fname, "wb"))
		return fprintf(stderr, "_pe_save: Error %d opening file '%s'\n", err, fname), 0;

	if (fwrite(pe->FileData(), 1, pe->FileLen(), file) != pe->FileLen())
		return fprintf(stderr, "_pe_save: Failed writing bytes to file '%s'\n", fname), 0;

	if (fclose(file) == EOF)
		return fprintf(stderr, "_pe_save: Failed to close file '%s'\n", fname), 0;

	return 0;
}

int CLuaPatcher::_ct_new(lua_State* L)
{
	CPortableExecutable* pe;
	CTraverse* code;

	if (!minilua_checkargs(L, "l", &pe) || !pe->IsValid())
		return fprintf(stderr, "_ct_new: Bad arguments\n"), 0;

	CLuaPatcher::PushNewInstance(L, new CTraverse(pe->ImgData(), pe->ImageBase(), pe->Is64bit()));

	return 1;
}

int CLuaPatcher::_ct_ndatas(lua_State* L)
{
	CTraverse* code;

	if (!minilua_checkargs(L, "l", &code))
		return fprintf(stderr, "_ct_ndatas: Bad arguments\n"), 0;

	lua_pushinteger(L, code->Data().size());
	return 1;
}

int CLuaPatcher::_ct_data(lua_State* L)
{
	CTraverse* code;
	int index;
	size_t i;
	const TraverseData* data = 0;

	if (!minilua_checkargs(L, "li", &code, &index) ||
		index < 1 || index > code->Data().size())
		return fprintf(stderr, "_ct_data: Bad arguments\n"), 0;

	i = 0;
	index -= 1; // LUA indexes start at 1
	for (auto it = code->Data().cbegin(); it != code->Data().cend() && i <= index; ++it, ++i)
	{
		if (i == index)
			data = &it->second;
	}

	PushTraverseData(L, data);
	return 1;
}

int CLuaPatcher::_ct_nfuncs(lua_State* L)
{
	CTraverse* code;
	if (!minilua_checkargs(L, "l", &code))
		return fprintf(stderr, "_ct_nfuncs: Bad arguments\n"), 0;

	lua_pushinteger(L, code->Funcs().size());
	return 1;
}

int CLuaPatcher::_ct_func(lua_State* L)
{
	CTraverse* code;
	int index;
	const TraverseFunc* func = 0;
	size_t i;

	if (!minilua_checkargs(L, "li", &code, &index)
		|| index < 1 || index > code->Funcs().size())
		return fprintf(stderr, "_ct_func: Bad arguments\n"), 0;

	i = 0;
	index -= 1; // LUA indexes start at 1
	for (auto it = code->Funcs().cbegin(); it != code->Funcs().cend() && i <= index; ++it, ++i)
	{
		if (i == index)
			func = &(*it);
	}

	PushTraverseFunc(L, func);
	return 1;
}

int CLuaPatcher::_ct_findfunc(lua_State* L)
{
	CTraverse* code;
	uint64_t* addr;
	const TraverseFunc* func;

	if (!minilua_checkargs(L, "lu", &code, &addr))
		return fprintf(stderr, "_ct_findfunc: Bad arguments\n"), 0;

	if (func = code->Find_Func(*(void**)addr))
		PushTraverseFunc(L, func);
	else lua_pushnil(L);

	return 1;
}

int CLuaPatcher::_ct_strref(lua_State* L)
{
	CTraverse* code;
	const char* str, * read;
	size_t slen, i;
	const TraverseData* data = 0;

	if (!minilua_checkargs(L, "ls", &code, &str))
		return fprintf(stderr, "_ct_strref: Bad arguments\n"), 0;

	lua_newtable(L);

	i = 1;
	slen = strlen(str) + 1;
	for (auto& pair : code->Data())
	{
		data = &pair.second;
		read = code->Read_String(data->loc, slen);
		if (read && !strcmp(read, str))
		{
			PushTraverseData(L, data);
			lua_rawseti(L, -2, i);
			++i;
		}

	}

	return 1;
}

int CLuaPatcher::_ct_addpage(lua_State* L)
{
	CTraverse* code;
	uint64_t* addr, * len;
	int flags;

	if (!minilua_checkargs(L, "luui", &code, &addr, &len, &flags))
		return fprintf(stderr, "_ct_addpage: Bad arguments\n"), 0;

	code->AddPage(*(void**)addr, *len, (uint8_t)flags);

	return 0;
}

int CLuaPatcher::_ct_traverse(lua_State* L)
{
	CTraverse* code;
	uint64_t* addr;
	size_t oldcount;

	if (!minilua_checkargs(L, "lu", &code, &addr))
		return fprintf(stderr, "_ct_traverse: Bad arguments\n"), 0;

	oldcount = code->Funcs().size();
	if (!code->Traverse_Func(*(void**)addr))
		return fprintf(stderr, "_ct_traverse: Failed to traverse location\n"), 0;

	//printf("_ct_traverse: %zu new functions found\n", code->Funcs().size() - oldcount);

	return 0;
}

int CLuaPatcher::_ct_readstr(lua_State* L)
{
	CTraverse* code;
	uint64_t* loc;
	int maxlen;
	const char* str;

	if (!minilua_checkargs(L, "lui", &code, &loc, &maxlen) || maxlen <= 0)
		return fprintf(stderr, "_ct_readstr: Bad arguments\n"), 0;

	lua_pushstring(L, code->Read_String(*(void**)loc, (size_t)maxlen + 1)); // Include null-terminator in size
	return 1;
}

int CLuaPatcher::_dialog_file(lua_State* L)
{
	const char* title = NULL, * path = NULL;
	char buf[MAX_PATH] = { 0 };

	path = lua_tostring(L, -2);
	title = lua_tostring(L, -1);

	lua_pushstring(L, Util::OpenFileDialog(buf, sizeof(buf), title, path)); // LUA handles NULL C strings
	return 1;
}

int CLuaPatcher::_dialog_msgbox(lua_State* L)
{
	const char* pszTitle = NULL;
	const char* pszContent = NULL;
	minilua_checkargs(L, "ss", &pszTitle, &pszContent); // Just reading the string easier, argument can be nil

	MessageBoxA(NULL, pszContent, pszTitle, NULL); // user32!MessageBoxA does all of the type-safe checking

	return 0;
}

void CLuaPatcher::PushInstance(lua_State* L, CLuaManagedPtrBase* Ptr)
{
	CLuaPatcher* _this;

	lua_getglobal(L, LUAPATCHER_INST);
	_this = (CLuaPatcher*)lua_touserdata(L, -1);

	assert(lua_islightuserdata(L, -1) && _this && "Failed to get CLuaPatcher instance from globals");

	_this->m_managed.push_back(Ptr);
	lua_pop(L, 1);

	lua_pushlightuserdata(L, Ptr);
}

void CLuaPatcher::PushTraverseFunc(lua_State* L, const TraverseFunc* Func)
{
	size_t i;

	lua_createtable(L, 0, 5);

	// len
	lua_pushinteger(L, Func->len);
	lua_setfield(L, -2, "len");

	//entry
	PushAddress(L, Func->entry);
	lua_setfield(L, -2, "entry");

	//low
	PushAddress(L, Func->low);
	lua_setfield(L, -2, "low");

	//name
	lua_pushstring(L, Func->name);
	lua_setfield(L, -2, "name");

	//blocks
	lua_createtable(L, Func->blocks.size(), 0);

	i = 1;
	for (auto it = Func->blocks.cbegin(); it != Func->blocks.cend(); ++it, ++i)
	{
		const TraverseBlock* block = *it;
		lua_createtable(L, 0, 2); // block

		lua_pushinteger(L, block->len);
		lua_setfield(L, -2, "len");

		PushAddress(L, block->loc);
		lua_setfield(L, -2, "loc");

		lua_pushstring(L, block->name);
		lua_setfield(L, -2, "name");

		lua_rawseti(L, -1, i); // blocks[i] = block
	}

	lua_setfield(L, -2, "blocks"); // Func.blocks = blocks
}

void CLuaPatcher::PushTraverseData(lua_State* L, const TraverseData* Data)
{
	size_t i = 0;

	lua_createtable(L, 0, 4);

	lua_pushstring(L, Data->name);
	lua_setfield(L, -2, "name"); // Data.name

	PushAddress(L, Data->loc);
	lua_setfield(L, -2, "loc"); // Data.loc

	lua_pushinteger(L, Data->type);
	lua_setfield(L, -2, "type"); // Data.type

	lua_createtable(L, Data->refs.size(), 0); // Ref addr table

	// Fill addr table
	i = 1;
	for (auto it = Data->refs.cbegin(); it != Data->refs.cend(); ++it, ++i)
	{
		PushAddress(L, *it);
		lua_rawseti(L, -2, i); // Data.refs[i]
	}

	lua_setfield(L, -2, "refs"); // Data.refs
}

void CLuaPatcher::PushAddress(lua_State* L, uint64_t Addr) {
	*(uint64_t*)lua_newuserdata(L, sizeof(Addr)) = Addr;
}

template <class T>
CPortableExecutable* LuaPatcher_pe_Tmember(lua_State* L, T** out_Data)
{
	CPortableExecutable* pe;
	T* usrdata;

	if (!minilua_checkargs(L, "l", &pe) || !pe->IsValid())
		return fprintf(stderr, "LuaPatcher::_pe_Tmember: Bad arguments\n"), nullptr;

	if (!(usrdata = (T*)lua_newuserdata(L, sizeof(T))))
		return 0;

	*out_Data = usrdata;
	return pe;
}

int minilua_checkargs(lua_State* L, const char* Fmt, ...)
{
	va_list va;
	int argc = strlen(Fmt);
	va_start(va, Fmt);

	for (int i = 0; Fmt[i]; ++i)
	{
		int type, idx = -(argc - i);
		switch (Fmt[i])
		{
		case 'i': type = LUA_TNUMBER; break;
		case 's': type = LUA_TSTRING; break;
		case 't': type = LUA_TTABLE; break;
		case 'u': type = LUA_TUSERDATA; break;
		case 'l': type = LUA_TLIGHTUSERDATA; break;
		default:
			return 0;
		}

		if (lua_type(L, idx) != type)
			return 0;

		switch (type)
		{
		case LUA_TNUMBER: *va_arg(va, int*) = lua_tointeger(L, idx); break;
		case LUA_TSTRING: *va_arg(va, const char**) = lua_tostring(L, idx); break;
		case LUA_TTABLE: *va_arg(va, int*) = idx; break;
		case LUA_TUSERDATA:
			if (!(*va_arg(va, void**) = lua_touserdata(L, idx)))
				return 0;
			break;
		case LUA_TLIGHTUSERDATA:
			if (!(*va_arg(va, void**) = ((CLuaManagedPtrBase*)lua_touserdata(L, idx))->_Inst()))
				return 0;
			break;
		default:
			return 0;
		}
	}

	va_end(va);
	return 1;
}

