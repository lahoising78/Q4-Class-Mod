#include "../idlib/precompiled.h"
#pragma hdrstop

#include "AbilityFF.h"

AbilityFF::AbilityFF(Ability type) {
	this->type = type;
}

void AbilityFF::Perform(CharacterFF* caster, CharacterFF* target) {
	gameLocal.Printf("Kinda calling the wrong function my dude\n");
	switch (type) {
	case CURE:
		CurePerform(caster, target);
	}

}

void AbilityFF::CurePerform(CharacterFF* caster, CharacterFF* target) {
	gameLocal.Printf("Cure perform\n");
	int mp = caster->mp / 5;

	int extra;
	if (target->maxhp < target->hp + effectivity + mp) {
		extra = target->maxhp - target->hp;
	}
	else {
		extra = effectivity + mp;
	}

	target->hp += extra;
}
/*
Cure::Cure(int effectivity, int cost) {
	gameLocal.Printf("Creating cure\n");
	this->effectivity = effectivity;
	this->cost = cost;
	this->name = "CURE";
}

void Cure::Perform(CharacterFF* caster, CharacterFF* target){
	gameLocal.Printf("Cure perform\n");
	int mp = caster->mp / 5;

	int extra;
	if (target->maxhp < target->hp + effectivity + mp) {
		extra = target->maxhp - target->hp;
	}
	else {
		extra = effectivity + mp;
	}

	target->hp += extra;

	// tHp += effectivity + mp;
}*/