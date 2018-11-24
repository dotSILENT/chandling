#pragma once
#include "game\game.h"

namespace HandlingMgr
{
	bool	InitVehicle(int vehicleID, int model); // this needs to be called each time a vehicle is created in game, before GetHandlingPtrForVehicle

	tHandlingData*	GetHandlingPtrForVehicle(int vehicleID, int model);

	bool	RecalculateModelHandlings(); // this gets only called once, after the game is done loading all the handlings

	void	InitializeModelDefaults(); // this needs to be called first

};