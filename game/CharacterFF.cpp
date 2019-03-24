#include "../idlib/precompiled.h"
#pragma hdrstop

#include "CharacterFF.h"

CharacterFF::CharacterFF(const char* name, int maxhp, ClassType classType){
	gameLocal.Printf("the passed string is: %s\n", name);
	this->name = name;
	gameLocal.Printf("the new enemy is called %s\n", this->name);
	this->maxhp = maxhp;
	hp = maxhp;
	this->classType = classType;
	atk = 3; //temporary default value
}

const char* CharacterFF::Attack(CharacterFF* target) {
	
	
	if (target->hp <= 0) {
		idStr msg = name;
		msg += " missed their target";
		return msg;
	}
	
	idStr msg = name;
	int dmg = atk;
	target->hp -= dmg;
	if (target->hp < 0) target->hp = 0;
	msg += " dealt ";
	msg += dmg;
	msg += " damage to ";
	msg += target->name;
	//gameLocal.Printf("%s dealt %d damage to %s\n", name, dmg, target->name);
	return msg;
}