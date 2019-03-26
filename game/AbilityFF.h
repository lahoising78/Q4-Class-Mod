#ifndef ABILITY_FF_H
#define ABILITY_FF_H

#include "CharacterFF.h"

enum Ability {
	CURE
};

class AbilityFF{

	public:
		AbilityFF(Ability type);
		int effectivity;
		int sa;
		int bc;
		int cost;
		idStr name;
		Ability type;
		void Perform(CharacterFF* caster, CharacterFF* target);
		void CurePerform(CharacterFF* caster, CharacterFF* target);
};

/* class Cure : public AbilityFF {
	public:
		Cure(int effectivity = 10, int cost = 1);
		void Perform(CharacterFF* caster, CharacterFF* target);
}; */

#endif 