#include "../idlib/precompiled.h"
#pragma hdrstop

#include "CharacterFF.h"

CharacterFF::CharacterFF(){
	name = "default name";
	maxhp = 10;
	hp = maxhp;
	classType = FIGHTER;
}

CharacterFF::CharacterFF(const char* name, int maxhp, ClassType classType){
	this->name = name;
	this->maxhp = maxhp;
	hp = maxhp;
	this->classType = classType;
	atk = 3; //temporary default value
}

void CharacterFF::Attack(CharacterFF* target) {
	target->hp -= atk;
	gameLocal.Printf("%s attacked %s\n", name, target->name);
}