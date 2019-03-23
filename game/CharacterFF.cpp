#include "../idlib/precompiled.h"
#pragma hdrstop

#include "CharacterFF.h"

CharacterFF::CharacterFF(const char* name, int maxhp, ClassType classType){
	this->name = name;
	this->maxhp = maxhp;
	hp = maxhp;
	this->classType = classType;
	atk = 3; //temporary default value
}

const char* CharacterFF::Attack(CharacterFF* target) {
	int dmg = atk;
	target->hp -= dmg;
	const char* msg = "someone dealt damage to someone";
	gameLocal.Printf("%s dealt %d damage to %s\n", name, dmg, target->name);
	return msg;
}