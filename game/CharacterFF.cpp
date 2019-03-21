#include "../idlib/precompiled.h"
#pragma hdrstop

#include "CharacterFF.h"

CharacterFF::CharacterFF(const char* name, int maxhp){
	this->name = name;
	this->maxhp = maxhp;
	hp = maxhp;
}