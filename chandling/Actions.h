#include "raknet/BitStream.h"

// idk why i even made this, guess I like unordered maps
// only one 'callback' can be attached to one action ID

typedef unsigned char BYTE;

enum CHandlingAction
{
	ACTION_INIT = 10, // This is the only packet that is sent  by the player, to indicate that we can speak to him
	ACTION_SET_VEHICLE_HANDLING = 15,
	ACTION_SET_MODEL_HANDLING = 20
};





struct ActionParams
{
	CHandlingAction actionID;
	BitStream* bsData;
};

typedef void(*tActionFunc)(struct ActionParams params);

struct ActionFunc
{
	tActionFunc func;
};

namespace Actions
{

	void Register(CHandlingAction id, tActionFunc func);
	bool Process(CHandlingAction actionID, BitStream* bsData);
};