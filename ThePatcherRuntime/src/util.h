#pragma once
#include <stddef.h>
#include <string.h>
#include <minilua.h>

#if !defined(MAX_PATH) // Use existing or default
#define MAX_PATH 260
#endif

#define HEXCHAR_ISHEX(c) ((c >= '0' && c <= '9') || (tolower(c) >= 'a' && tolower(c) <= 'f'))
#define HEXCHAR_TOHEX(c) ((c >= '0' && c <= '9') ? c - '0' : tolower(c) - 'a' + 0xa)

namespace Util
{
	char* GetFileData(const char* szFile, size_t* out_Len);
	char* GetResourceData(int ResId, const char* ResType, size_t* out_Len);
	char* OpenFileDialog(char* Buffer, size_t BufLen, const char* Title = NULL, const char* InitialDir = NULL);
	void  DumpStack(lua_State* L);
}