#include "../idlib/precompiled.h"
#pragma hdrstop

#include "CharacterFF.h"

CharacterFF::CharacterFF(){
	name = "default name";
	maxhp = 10;
	hp = maxhp;
	classType = FIGHTER;
	atk = 3;
}

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
	char* msg;
	idStr builder;
	builder.snPrintf(msg, 50, "%s dealt %d damage to %s", name, dmg, target->name);
	const char* ret = msg;
	return ret;
}