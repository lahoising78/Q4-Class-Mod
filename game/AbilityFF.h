#ifndef ABILITY_FF_H
#define ABILITY_FF_H

#include "CharacterFF.h"

class AbilityFF{

	public:
		int effectivity;
		int sa;
		int bc;
		int cost;
		idStr name;
		virtual void Perform(CharacterFF* caster, CharacterFF* target);
};

class Cure : public AbilityFF {
	public:
		Cure(int effectivity = 10, int cost = 1);
		void Perform(CharacterFF* caster, CharacterFF* target);
};

#endif 