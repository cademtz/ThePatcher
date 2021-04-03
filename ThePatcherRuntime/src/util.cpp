#include "util.h"
#include <stdio.h>
#include <stdlib.h>

// Reason: Resource files
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

char* Util_GetFileData(const char* szFile, size_t* out_Len)
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

char* Util_GetResourceData(int ResId, const char* ResType, size_t* out_Len)
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
