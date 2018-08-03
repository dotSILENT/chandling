#ifndef _HOOKFUNCTIONS_H
#define _HOOKFUNCTIONS_H

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <Psapi.h>

#pragma comment(lib, "Psapi.lib")

void Patch(LPVOID address, LPVOID patch, int num);
void NOP(LPVOID address, int num);
void VTableRedirect(LPVOID* vtbl, LPVOID hooked, LPVOID* origin, int idx);
LPVOID DetourFunc(BYTE * Source, BYTE * Destination, int numbytes);
VOID RetourFunc(BYTE * Source, BYTE ** AllocJmp, int numbytes);

bool memory_compare(const BYTE *data, const BYTE *pattern, const char *mask);
DWORD FindPattern(char *pattern, char *mask);

#endif