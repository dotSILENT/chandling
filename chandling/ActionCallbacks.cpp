#include "Actions.h"
#include "main.h"

void OnSetVehicleHandling(ActionParams p)
{
	DebugPrint("OnSetVehicleHandling\n");
}

void OnResetVehicle(ActionParams p)
{
	DebugPrint("OnResetVehicle");
}

void OnSetModelHandling(ActionParams p)
{
	DebugPrint("OnSetModelHandling");
}

void OnResetModel(ActionParams p)
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