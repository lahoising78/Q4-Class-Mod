#include "../idlib/precompiled.h"
#pragma hdrstop

#include "CharacterFF.h"

CharacterFF::CharacterFF(){
	name = "a name";
	maxhp = 10;
	hp = maxhp;
	classType = FIGHTER;
}

CharacterFF::CharacterFF(const char* name, int maxhp, ClassType classType){
	this->name = name;
	this->maxhp = maxhp;
	hp = maxhp;
	this->classType = classType;
}

void CharacterFF::Attack(CharacterFF* target) {
	target->hp -= atk;
}