#include "../idlib/precompiled.h"
#pragma hdrstop

#include "AbilityFF.h"

void AbilityFF::Perform(CharacterFF* caster, CharacterFF* target) {
	gameLocal.Printf("Kinda calling the wrong function my dude\n");
}

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
}