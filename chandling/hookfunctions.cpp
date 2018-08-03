#define WIN32_LEAN_AND_MEAN


#include "hookfunctions.h"


bool memory_compare(const BYTE *data, const BYTE *pattern, const char *mask)
{
	for (; *mask; ++mask, ++data, ++pattern)
	{
		if (*mask == 'x' && *data != *pattern)
			return false;
	}
	return (*mask) == NULL;
}

DWORD FindPattern(char *pattern, char *mask)
{
	DWORD i;
	DWORD size;
	DWORD address;

	MODULEINFO info = { 0 };

	address = (DWORD)GetModuleHandleA("samp.dll");
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA("samp.dll"), &info, sizeof(MODULEINFO));
	size = (DWORD)info.SizeOfImage;

	for (i = 0; i < size; ++i)
	{
		if (memory_compare((BYTE *)(address + i), (BYTE *)pattern, mask))
			return (DWORD)(address + i);
	}
	return 0;
}


void Patch(LPVOID address, LPVOID patch, int num)
{
	DWORD dwOldProt;
	VirtualProtect(address, num, PAGE_EXECUTE_READWRITE, &dwOldProt);
	memcpy(address, patch, num);
	VirtualProtect(address, num, dwOldProt, &dwOldProt);
}

void NOP(LPVOID address, int num)
{
	DWORD dwOldProt;
	int i;
	VirtualProtect(address, num, PAGE_EXECUTE_READWRITE, &dwOldProt);
	for (i = 0; i < num; i++)
		((BYTE*)address)[i] = 0x90;
	VirtualProtect(address, num, dwOldProt, &dwOldProt);
}

void VTableRedirect(LPVOID* vtbl, LPVOID hooked, LPVOID* origin, int idx)
{
	DWORD dwOldProt;
	VirtualProtect(&vtbl[idx], sizeof(LPVOID), PAGE_EXECUTE_READWRITE, &dwOldProt);
	if (origin)
		*origin = vtbl[idx];
	vtbl[idx] = hooked;
}

LPVOID DetourFunc(BYTE * Source, BYTE * Destination, int numbytes)
{
	BYTE * AllocJmp;
	DWORD dwProtect;
	DWORD JmpOff;
	DWORD val;

	if (Source == NULL) return NULL;
	if (Destination == NULL) return NULL;
	if (numbytes < 5) return NULL;

	AllocJmp = (BYTE*)malloc(5 + numbytes);

	VirtualProtect(AllocJmp, 5 + numbytes, PAGE_EXECUTE_READWRITE, &dwProtect);
	VirtualProtect(Source, numbytes, PAGE_EXECUTE_READWRITE, &dwProtect);

	memcpy(AllocJmp, Source, numbytes);

	if (AllocJmp[0] == 0xE9 || AllocJmp[0] == 0xE8)
	{
		val = *(DWORD*)(&AllocJmp[1]);
		*(DWORD*)(&AllocJmp[1]) = val + Source - AllocJmp;
	}

	AllocJmp += numbytes;
	DWORD JmpToOrigin = (DWORD)((Source + numbytes) - (AllocJmp + 5));

	AllocJmp[0] = 0xE9;
	*(DWORD*)(AllocJmp + 1) = JmpToOrigin;

	JmpOff = (DWORD)(Destination - Source) - 5;

	AllocJmp -= numbytes;

	Source[0] = 0xE9;
	*(DWORD*)(Source + 1) = JmpOff;

	VirtualProtect(Source, numbytes, dwProtect, &dwProtect);

	return AllocJmp;
}

VOID RetourFunc(BYTE * Source, BYTE ** AllocJmp, int numbytes)
{
	DWORD dwProtect;

	if (AllocJmp == NULL) return;
	if (*AllocJmp == NULL) return;
	if (Source == NULL) return;
	if (numbytes < 5) return;

	VirtualProtect(Source, numbytes, PAGE_EXECUTE_READWRITE, &dwProtect);
	memcpy(Source, *AllocJmp, numbytes);

	if (Source[0] == 0xE9 || Source[0] == 0xE8)
	{
		DWORD val = *(DWORD*)(&Source[1]);
		*(DWORD*)(&Source[1]) = val + (*AllocJmp) - Source;
	}

	VirtualProtect(Source, numbytes, dwProtect, &dwProtect);

	free(*AllocJmp);
	*AllocJmp = NULL;
}