#include "../idlib/precompiled.h"
#pragma hdrstop

#include "AbilityFF.h"

AbilityFF::AbilityFF(Ability type, int effectivity, int cost) {
	this->type = type;
	this->effectivity = effectivity;
	this->cost = cost;
}

const char* AbilityFF::Perform(CharacterFF* caster, CharacterFF* target) {
	gameLocal.Printf("Kinda calling the wrong function my dude\n");
	switch (type) {
	case CURE:
		return CurePerform(caster, target);
		break;
	case FIRE:
		return FirePerform(caster, target);
		break;
	}

}

const char* AbilityFF::CurePerform(CharacterFF* caster, CharacterFF* target) {
	gameLocal.Printf("Cure perform\n");
	idStr msg;
	int mp = caster->mp / 5;

	int extra;
	if (target->maxhp < target->hp + effectivity + mp) {
		extra = target->maxhp - target->hp;
	}
	else {
		extra = effectivity + mp;
	}

	target->hp += extra;
	caster->mp -= cost;

	msg = caster->name;
	msg += " cured ";
	msg += extra;
	msg += " health on ";
	msg += target->name;
	//const char* msg = "ete e cure klk no bulto";

	return msg;
}

const char* AbilityFF::FirePerform(CharacterFF* caster, CharacterFF* target) {
	gameLocal.Printf("Fire perform\n");
	if (target->hp <= 0) {
		idStr msg = name;
		msg += " missed their target";
		return msg;
	}
	
	//idStr msg;
	int mp = caster->mp / 5;
	
	//target->hp -= dmg;
	caster->mp -= cost;

	idStr msg = name;
	int dmg = effectivity + mp;
	target->hp -= dmg;
	if (target->hp < 0) target->hp = 0;
	//char* msg = "Hola soy german";
	//= caster->name;
	msg += " dealt ";
	msg += dmg;
	msg += " damage to ";
	msg += target->name;
	gameLocal.Printf("effect: %d, mp: %d, damage: %d\n", effectivity, mp, dmg);

	return msg;
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