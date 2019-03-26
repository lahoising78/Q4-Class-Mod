#ifndef ABILITY_FF_H
#define ABILITY_FF_H

#include "CharacterFF.h"

enum Ability {
	CURE,
	FIRE
};

class AbilityFF{

	public:
		AbilityFF(Ability type, int effectivity = 10, int cost = 1);
		int effectivity;
		int sa;
		int bc;
		int cost;
		idStr name;
		Ability type;
		const char* Perform(CharacterFF* caster, CharacterFF* target);
		const char* CurePerform(CharacterFF* caster, CharacterFF* target);
		const char* FirePerform(CharacterFF* caster, CharacterFF* target);
};

/* class Cure : public AbilityFF {
	public:
		Cure(int effectivity = 10, int cost = 1);
		void Perform(CharacterFF* caster, CharacterFF* target);
}; */

#endif 