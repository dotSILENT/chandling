#pragma once
#include "main.h"
#include "raknet/RakClientInterface.h"
#include "game/game.h"

typedef CVehicle*(__cdecl *tCreateCar)(DWORD model, CVector pos, BYTE mission);
typedef int(__thiscall *tSampIDFromGtaPtr)(DWORD* thisptr, int gtaptr);
typedef int(__thiscall *tSampCreateVehicle)(DWORD* thisptr, int vehiclesmth);
typedef CVehicle*(__stdcall *tSampGetVehicleStruct)(int internalHandle);

typedef Packet*(__thiscall *tReceive)(RakClientInterface* thisptr);

void SetupGtaHooks();
bool SetupSampHooks();