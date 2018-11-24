#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdarg.h>
#include <cstdio>
#include "game/game.h"

#define DEBUG

/* Uncomment this to save all the handlings from handling.cfg (mapped to their models) to a file */
//#define DUMP_DEFAULT_HANDLINGS

#ifdef DEBUG
#define DebugPrint(str,...) printf("[cHandling] " str "\n", __VA_ARGS__)
#else 
#define DebugPrint(...)
#endif

#define MAX_VEHICLES (2000)
#define MAX_VEHICLE_MODELS (212)

#define IS_VALID_VEHICLEID(id) \
	(id >= 1 && id <= MAX_VEHICLES)

#define IS_VALID_VEHICLE_MODEL(modelid) \
	(modelid >= 400 && modelid < MAX_VEHICLE_MODELS+400)

#define VEHICLE_MODEL_INDEX(modelid) \
	(modelid - 400)


#include "CAddresses.h"
extern CAddresses Addr;
extern bool gInited;
extern DWORD dwSampDLL;
extern CVehicle** pID2PTR;

