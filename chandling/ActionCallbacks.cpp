#include "ActionCallbacks.h"
#include "main.h"
#include "Actions.h"
#include <cstdint>
#include "HandlingManager.h"
//#include "HandlingEnum.h"

void OnServerInit(ActionParams* p)
{
	DebugPrint("OnServerInit");

	uint32_t compat_ver;
	p->bsData->Read(compat_ver);
	bool allowed = false;
	p->bsData->Read(allowed);

	if (!allowed)
	{
		// Client has older compatibility version, cannot use CHandling
		LogError("Incorrect compatibility version (client: 0x%x, server: 0x%x), update the plugin!", CHANDLING_COMPAT_VERSION, compat_ver);
		return;
	}

	// allow handling modifications
	gUsingCHandling = true;
}

void OnSetVehicleHandling(ActionParams* p)
{
	DebugPrint("OnSetVehicleHandling");
	if (!gUsingCHandling || p->bsData->GetNumberOfBytesUsed() < sizeof(uint16_t) + sizeof(uint8_t))
		return;

	uint16_t vehicleid = 0;
	uint8_t count = 0, attrib, type;

	struct stHandlingMod mod;

	p->bsData->Read(vehicleid);
	p->bsData->Read(count);

	bool ok = true;

	for (int i = 0; i < count; i++)
	{
		p->bsData->Read(attrib);
		ok = p->bsData->Read(type);
		mod.type = (CHandlingAttribType)type;
		mod.attrib = (CHandlingAttrib)attrib;
		switch ((CHandlingAttribType)type)
		{
		case TYPE_BYTE:
			ok = p->bsData->Read(mod.bval);
			break;
		case TYPE_UINT:
		case TYPE_FLAG:
			ok = p->bsData->Read(mod.uival);
			break;
		case TYPE_FLOAT:
			ok = p->bsData->Read(mod.fval);
			break;
		}

		if (!ok)
			return;

		HandlingMgr::AddVehicleMod(vehicleid, mod);
		
	}
	HandlingMgr::ApplyVehicleMods(vehicleid);
}

void OnResetVehicle(ActionParams* p)
{
	if (!gUsingCHandling || p->bsData->GetNumberOfBytesUsed() < sizeof(uint16_t))
		return;
	uint16_t vehicleid = 0;
	p->bsData->Read(vehicleid);

	HandlingMgr::InitVehicle(vehicleid, -1, true);
	DebugPrint("OnResetVehicle");
}

void OnSetModelHandling(ActionParams* p)
{
	if (!gUsingCHandling || p->bsData->GetNumberOfBytesUsed() < sizeof(uint16_t) + sizeof(uint8_t))
		return;
	
	uint16_t modelid = 0;
	uint8_t count = 0, attrib, type;

	struct stHandlingMod mod;

	p->bsData->Read(modelid);
	p->bsData->Read(count);
	bool ok = true;

	for (int i = 0; i < count; i++)
	{
		p->bsData->Read(attrib);
		ok = p->bsData->Read(type);
		mod.type = (CHandlingAttribType)type;
		mod.attrib = (CHandlingAttrib)attrib;
		switch ((CHandlingAttribType)type)
		{
		case TYPE_BYTE:
			ok = p->bsData->Read(mod.bval);
			break;
		case TYPE_UINT:
		case TYPE_FLAG:
			ok = p->bsData->Read(mod.uival);
			break;
		case TYPE_FLOAT:
			ok = p->bsData->Read(mod.fval);
			break;
		}

		if (!ok)
			return;
		HandlingMgr::AddModelMod(modelid, mod);
	}
	HandlingMgr::ApplyModelMods(modelid);
	DebugPrint("OnSetModelHandling %d count %d", modelid, count);
}

void OnResetModel(ActionParams* p)
{
	if (!gUsingCHandling || p->bsData->GetNumberOfBytesUsed() < sizeof(uint16_t))
		return;
	uint16_t modelid = 0;
	p->bsData->Read(modelid);

	if (!IS_VALID_VEHICLE_MODEL(modelid))
		return;
	
	HandlingMgr::InitializeModelDefaults(modelid);
	DebugPrint("OnResetModel");
}

void RegisterAllActionCallbacks()
{
	Actions::Register(ACTION_INIT_RESPONSE, OnServerInit);
	Actions::Register(ACTION_RESET_VEHICLE, OnResetVehicle);
	Actions::Register(ACTION_SET_VEHICLE_HANDLING, OnSetVehicleHandling);
	Actions::Register(ACTION_SET_MODEL_HANDLING, OnSetModelHandling);
	Actions::Register(ACTION_RESET_MODEL, OnResetModel);
}