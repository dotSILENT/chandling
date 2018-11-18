#include "Actions.h"
#include "main.h"

void OnSetVehicleHandling(ActionParams p)
{
	DebugPrint("OnSetVehicleHandling\n");
}

void RegisterAllActionCallbacks()
{
	Actions::Register(CHandlingAction::ACTION_SET_VEHICLE_HANDLING, OnSetVehicleHandling);
}