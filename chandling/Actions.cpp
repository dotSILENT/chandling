#include "Actions.h"
#include "main.h"
#include <unordered_map>

static std::unordered_map<CHandlingAction, struct ActionFunc> actionMap;

namespace Actions
{
	void Register(CHandlingAction id, tActionFunc func)
	{
		struct ActionFunc x;
		x.func = func;
		actionMap[id] = x;
	}


	bool Process(CHandlingAction actionID, BitStream* bsData)
	{
		if (actionMap.count(actionID))
		{
			struct ActionParams params;
			params.actionID = actionID;
			params.bsData = bsData;

			actionMap[actionID].func(params);
			return true;
		}
		return false;
	}
}