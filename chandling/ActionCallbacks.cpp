#include "main.h"
#include "Actions.h"
#include <cstdint>

void OnSetVehicleHandling(ActionParams* p)
{
	DebugPrint("OnSetVehicleHandling\n");
}

void OnResetVehicle(ActionParams* p)
{
	if (p->bsData->GetNumberOfBytesUsed() < sizeof(uint16_t))
		return;
	uint16_t vehicleid = 0;
	p->bsData->Read(vehicleid);
	DebugPrint("OnResetVehicle");
}

void OnSetModelHandling(ActionParams* p)
{
	DebugPrint("OnSetModelHandling");
}

void OnResetModel(ActionParams* p)
{
	DebugPrint("OnResetModel");
}

void RegisterAllActionCallbacks()
{
	Actions::Register(ACTION_RESET_VEHICLE, OnResetVehicle);
	Actions::Register(ACTION_SET_VEHICLE_HANDLING, OnSetVehicleHandling);
	Actions::Register(ACTION_SET_MODEL_HANDLING, OnSetModelHandling);
	Actions::Register(ACTION_RESET_MODEL, OnResetModel);
}