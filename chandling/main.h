#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdarg.h>
#include <cstdio>

#define DEBUG

/* Uncomment this to save all the handlings from handling.cfg (mapped to their models) to a file */
//#define DUMP_DEFAULT_HANDLINGS

#ifdef DEBUG
#define DebugPrint(...) printf("[cHandling] " __VA_ARGS__)
#else 
#define DebugPrint(...)
#endif

#define MAX_VEHICLES (2000)
#define MAX_MODELS (212)

#include "CAddresses.h"
extern CAddresses Addr;
extern bool gInited;
extern DWORD dwSampDLL;

