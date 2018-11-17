#pragma once
#include <Windows.h>
#include <Psapi.h>

bool memory_compare(const BYTE *data, const BYTE *pattern, const char *mask)
{
	for (; *mask; ++mask, ++data, ++pattern)
	{
		if (*mask == 'x' && *data != *pattern)
			return false;
	}
	return (*mask) == NULL;
}

DWORD FindPattern(DWORD module, char *pattern, char *mask)
{
	DWORD i;
	DWORD size;

	MODULEINFO info = { 0 };

	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("samp.dll"), &info, sizeof(MODULEINFO));
	size = (DWORD)info.SizeOfImage;

	for (i = 0; i < size; ++i)
	{
		if (memory_compare((BYTE *)(module + i), (BYTE *)pattern, mask))
			return (DWORD)(module + i);
	}
	return 0;
}