#include "main.h"
#include "HandlingManager.h"
#include "HandlingDefault.h"

namespace HandlingMgr
{
	void FUNC_CalcHandling(struct tHandlingData* handling)
	{
		((void(__stdcall *)(struct tHandlingData *))0x6F5080)(handling);
	}

	class CHandlingEntry
	{
	private:
		bool calculated = false;

		void RestoreUncalculated() // this restores the uncalculated handling & allows us to calculate it again
		{
			memcpy(&calcHandling, &rawHandling, sizeof(struct tHandlingData));
			calculated = false;
		}

	public:

		struct tHandlingData	rawHandling;
		struct tHandlingData	calcHandling;

		bool isCalculated() { return calculated; }

		void Recalculate()
		{
			RestoreUncalculated();
			FUNC_CalcHandling(&calcHandling);
			calculated = true;
		}

		void Clear()
		{
			calculated = false;
		}
	};

	struct CVehicleHandlingEntry : CHandlingEntry
	{
	public:
		bool	usesModelHandling = true;
		int		modelID = -1;
		struct tHandlingData* pCurrentHandling = nullptr;
	};





	bool modelsInitialized = false;
	CHandlingEntry modelHandlings[MAX_VEHICLE_MODELS]; // our per-model handlings

	CVehicleHandlingEntry vehicleHandlings[MAX_VEHICLES + 1]; // per-vehicle handlings

	bool InitVehicle(int vehicleID, int model)
	{
		if (!IS_VALID_VEHICLEID(vehicleID) || !IS_VALID_VEHICLE_MODEL(model))
			return false;

		vehicleHandlings[vehicleID].pCurrentHandling = &modelHandlings[VEHICLE_MODEL_INDEX(model)].calcHandling;
		DebugPrint("pcurrent 0x%x\n", (int)vehicleHandlings[vehicleID].pCurrentHandling);
		vehicleHandlings[vehicleID].usesModelHandling = true;
		vehicleHandlings[vehicleID].modelID = model;
		// copy the model's handling as our handling
		memcpy(&vehicleHandlings[vehicleID].rawHandling, &modelHandlings[VEHICLE_MODEL_INDEX(model)].rawHandling, sizeof(struct tHandlingData));
		vehicleHandlings[vehicleID].Recalculate();
		return true;
	}

	tHandlingData* GetHandlingPtrForVehicle(int vehicleID, int model)
	{
		if (!IS_VALID_VEHICLEID(vehicleID) || !IS_VALID_VEHICLE_MODEL(model))
			return nullptr;

		if (!vehicleHandlings[vehicleID].isCalculated())
			InitVehicle(vehicleID, model);

		return vehicleHandlings[vehicleID].pCurrentHandling;
	}

	bool RecalculateModelHandlings() // this gets only called once, after the game is done loading all the handlings
	{
		for (int i = 0; i < MAX_VEHICLE_MODELS; i++)
		{
			modelHandlings[i].Recalculate();
		}
		return true;
	}

	void InitializeModelDefaults()
	{
		for (int i = 0; i < MAX_VEHICLE_MODELS; i++)
		{
			HandlingDefault::copyDefaultModelHandling(i + 400, &modelHandlings[i].rawHandling);
		}

		RecalculateModelHandlings();
		modelsInitialized = true;
	}

}