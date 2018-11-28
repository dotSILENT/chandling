#pragma once
#include "game\game.h"
#include "HandlingEnum.h"

struct stHandlingMod
{
	CHandlingAttrib		attrib;
	CHandlingAttribType	type;
	union
	{
		float				fval;
		unsigned int		uival;
		uint8_t				bval;
	};
};

namespace HandlingMgr
{
	bool	InitVehicle(int vehicleID, int model, bool ignoreModel = false); // this needs to be called each time a vehicle is created in game, before GetHandlingPtrForVehicle

	tHandlingData*	GetHandlingPtrForVehicle(int vehicleID, int model);

	void	InitializeModelDefaults(uint16_t modelid = 0); // this needs to be called first

	void	RecalculateVehicle(int vehicleID);

	bool	ApplyVehicleMods(int vehicleID); // call after done adding mods with AddVehicleMod so the handling is recalculated & applied if vehicle exists

	bool	AddVehicleMod(int vehicleID, const struct stHandlingMod mod); // this just applies the mod to rawHandling, you need to run RecalculateVehicle() on your own

};