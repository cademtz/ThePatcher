#include <stdio.h>
#include <codetraverse/portable_executable.h>
#include <codetraverse/traverse.h>
#include <resource.h>
#include "lua.h"
#include "util.h"
#include "luapatcher.h"

#pragma comment(lib, "Zydis.lib")

int main(int argc, char** argv)
{
	int err = 0;
	lua_State* L;
	CLuaPatcher* patcher;
	const char*	pe_lua;
	size_t		pe_lua_len;

	if (argc < 2)
	{
		perror("Incorrect usage\nThePatcherRuntime <lua file>\n");
		return -1;
	}

	if (!(pe_lua = Util::GetResourceData(IDR_LUA_PE, "LUA", &pe_lua_len)))
	{
		perror("Failed to load PortableExecutable.lua");
		return -1;
	}

	L = luaL_newstate();
	luaL_openlibs(L);

	if (!L)
	{
		perror("Failed to initialize LUA");
		return -1;
	}

	patcher = new CLuaPatcher(L);

	if (luaL_loadbuffer(L, pe_lua, pe_lua_len, "PortableExecutable.lua") ||
		lua_pcall(L, 0, 0, 0))
	{
		fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
		return err = 1;
	}

	lua_createtable(L, 0, 1);
	lua_pushstring(L, argv[1]);
	lua_rawseti(L, -2, 1);
	lua_setglobal(L, "arg");

	if (luaL_loadfile(L, argv[1]) || lua_pcall(L, 0, 0, 0))
	{
		fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
		err = 1;
	}

	delete patcher;
	lua_close(L);

	system("pause");

	return err;
}
