#include "util.h"
#include <stdio.h>
#include <stdlib.h>

// Reason: Resource files
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shobjidl.h> 
#include <commdlg.h>

char* Util::GetFileData(const char* szFile, size_t* out_Len)
{
	FILE* file;
	size_t len;
	char* buf;

	if (fopen_s(&file, szFile, "rb"))
		return 0;

	fseek(file, 0, SEEK_END);
	len = ftell(file);
	rewind(file);

	if (!(buf = (char*)malloc(len)) ||fread(buf, sizeof(buf[0]), len, file) != len)
		return 0;

	fclose(file);

	*out_Len = len;
	return buf;
}

char* Util::GetResourceData(int ResId, const char* ResType, size_t* out_Len)
{
	HMODULE hMod;
	HRSRC	hResInfo;
	HGLOBAL	hResData;
	DWORD	size;
	void*	raw;
	
	if (!(hMod = GetModuleHandle(0)))
		return 0;

	if (!(hResInfo = FindResourceA(hMod, MAKEINTRESOURCEA(ResId), ResType)))
		return 0;

	if (!(hResData = LoadResource(hMod, hResInfo)))
		return 0;

	size = SizeofResource(hMod, hResInfo);
	raw = LockResource(hResData);

	*out_Len = (size_t)size;
    return (char*)raw;
}

char* Util::OpenFileDialog(char* Buffer, size_t BufLen, const char* Title, const char* InitialDir)
{
	OPENFILENAME file;
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.lpstrFile = Buffer;
	file.nMaxFile = BufLen;
	file.lpstrTitle = Title ? Title : "Patcher file open dialog";
	file.lpstrInitialDir = InitialDir;

	if (!GetOpenFileNameA(&file))
		return NULL;
	
	return Buffer;
}

void Util::DumpStack(lua_State* L)
{
	printf("Dumped stack: ");
	int top = lua_gettop(L);
	for (int i = 0; i < top; i++)
		printf(i == top - 1 ? "%s\n" : "%s, ", lua_typename(L, lua_type(L, i)));
}