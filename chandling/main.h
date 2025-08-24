#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdarg>
#include <cstdio>
#include "game/game.h"

#define CHANDLING_VERSION "v1.1"
#define CHANDLING_COMPAT_VERSION 0x1001D

#define CHANDLING_GITHUB_REPO "dotSILENT/chandling"

#define CHANDLING_ERROR_LOG "chandling_errors.log"

//#define DEBUG

/* Uncomment this to save all the handlings from handling.cfg (mapped to their models) to a file */
//#define DUMP_DEFAULT_HANDLINGS

#ifdef DEBUG
#define DebugPrint(str,...) printf("[cHandling] " str "\n", __VA_ARGS__);AddChatMessage(str, __VA_ARGS__)
#else 
#define DebugPrint(...)
#endif

void LogError(const char *fmt, ...);

#define MAX_VEHICLES (2000)
#define MAX_VEHICLE_MODELS (212)

#define IS_VALID_VEHICLEID(id) \
	(id >= 1 && id <= MAX_VEHICLES)

#define IS_VALID_VEHICLE_MODEL(modelid) \
	(modelid >= 400 && modelid < MAX_VEHICLE_MODELS+400)

#define VEHICLE_MODEL_INDEX(modelid) \
	(modelid - 400)

typedef unsigned long DWORD; // just so we dont include whole Windows.h here

extern bool gInited;
extern bool gUsingCHandling;
extern DWORD dwSampDLL;
extern CVehicle** pID2PTR;

typedef int(__cdecl *tAddChatMessage)(DWORD pChat, const char* fmt, ...);
extern DWORD *pSampChat;
extern tAddChatMessage addChatMsgFn;

void AddChatMessageRaw(const char* fmt, ...);
#define AddChatMessage(fmt,...) AddChatMessageRaw("[cHandling] " fmt, __VA_ARGS__)