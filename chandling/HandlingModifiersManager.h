#pragma once
#include "main.h"
#include "HandlingEnum.h"
#include "game/game.h"
#include <vector>

// A single modifier for single handling value
struct stHandlingModifier
{
	eHandlingAttribute attrib;
	char modifier; // -+*
	char value[16];
};

// a manager class which keeps all the modifiers for a single handling entry
class CHandlingModifiersMgr
{
private:
	std::vector<struct stHandlingModifier> vModifiers;

	void PushModifierInternal(struct stHandlingModifier mod)
	{
		this->vModifiers.push_back(mod);
	}

	void ApplyModifier(struct stHandlingModifier mod, float &handlval);

	void ApplyModifier(struct stHandlingModifier mod, int &handlval);

	void ApplyModifier(struct stHandlingModifier mod, unsigned int &handlval);

	void ApplyModifier(struct stHandlingModifier mod, uint8_t &handlval);

	void ApplyFlagModifier(struct stHandlingModifier mod, uint32_t &handlval);

public:
	CHandlingModifiersMgr()
	{
		vModifiers.reserve(10);
	}

	void ClearModifiers()
	{
		vModifiers.clear();
	}

	void AddModifier(eHandlingAttribute attrib, char*  value);

	void AddModifier(eHandlingAttribute attrib, int value, char modifier = NULL);

	/*bool RemoveModifierByValue(eHandlingAttribute attrib, char* value) // after removing a modifier, ApplyModifiersToHandling needs to be used again to apply every modifier but this one
	{
	std::list<struct stHandlingModifier>::iterator i = vModifiers.begin();

	while (i != vModifiers.end())
	{
	if (i->attrib == attrib && !strncmp(i->value, value, sizeof(stHandlingModifier::value)))
	{
	vModifiers.erase(i);
	return true; // there may be more modifiers of the same value & attribute, but we consider only one (since modifiers can be applied many times)
	}
	i++;
	}

	return false;
	}*/

	void ApplyModifiersToRawHandling(struct tHandlingData* handling); // raw handling, obviously
};