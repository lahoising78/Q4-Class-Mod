#include "../idlib/precompiled.h"
#pragma hdrstop

#include "CharacterFF.h"

CharacterFF::CharacterFF(const char* name, int maxhp, ClassType classType){
	gameLocal.Printf("the passed string is: %s\n", name);
	this->name = name;
	gameLocal.Printf("the new enemy is called %s\n", this->name);
	this->maxhp = maxhp;
	hp = maxhp;
	lv = 1;
	this->classType = classType;
	switch (classType) {
		case FIGHTER:
			CreateFighter();
			break;
		case BL_MAGE:
			CreateBlMage();
			break;
		case WT_MAGE:
			CreateWtMage();
			break;
	}
}

void CharacterFF::CreateFighter(){
	str = 20;
	agi = 5;
	intel = 1;
	vit = 10;
	lck = 5;
	hitp = 10;
	md = 15;
}

void CharacterFF::CreateWtMage(){
	str = 5;
	agi = 5;
	intel = 15;
	vit = 10;
	lck = 5;
	hitp = 5;
	md = 20;
}

void CharacterFF::CreateBlMage() {
	str = 1;
	agi = 10;
	intel = 20;
	vit = 1;
	lck = 10;
	hitp = 5;
	md = 20;
}

const char* CharacterFF::Attack(CharacterFF* target) {
	if (target->hp <= 0) {
		idStr msg = name;
		msg += " missed their target";
		return msg;
	}
	
	idStr msg = name;
	int dmg = str;
	target->hp -= dmg;
	if (target->hp < 0) target->hp = 0;
	msg += " dealt ";
	msg += dmg;
	msg += " damage to ";
	msg += target->name;
	//gameLocal.Printf("%s dealt %d damage to %s\n", name, dmg, target->name);
	return msg;
}

void CharacterFF::GainExperience(int n) {
	int gainedExp = n * 35;
	gameLocal.Printf("gaining %d experience\n", gainedExp);
	exp += gainedExp;

	if (exp > (40 << (lv - 1))){
		lv++;
		gameLocal.Printf("Leveled up to lvl %d!!\n", lv);

		switch (classType)
		{
			case FIGHTER:
				LevelUpFighter();
				break;
			case WT_MAGE:
				LevelUpWt();
				break;
			case BL_MAGE:
				LevelUpBl();
				break;
		}
	}
}

void CharacterFF::LevelUpFighter(){
	int oldmaxhp = maxhp;
	maxhp += vit / 4;
	if (((lv > 23) && (lv % 3 == 2)) || 
		((lv <= 23) && (lv > 11) && (lv % 3 != 0)) || 
		( lv <= 11)) {
		maxhp += 23;
	}
	gameLocal.Printf("+%d HP!\n", maxhp - oldmaxhp);

	str++;
	gameLocal.Printf("+1 STR!\n");

	int randagi = rand() % 100;
	if ( ((lv > 29) && (lv % 2 == 1)) || 
		 ((lv <= 29) && (lv > 20) && (lv % 3 != 0)) ||
		 (lv <= 20) || (randagi < 25) ){
		agi++;
		gameLocal.Printf("+1 AGI!\n");
	}

	int randintel = rand() % 100;
	if (((lv < 44) && (lv % 3 == 1)) || (randintel < 25)) {
		intel++;
		gameLocal.Printf("+1 INT!\n");
	}

	int randvit = rand() % 100;
	if (((lv < 41) && (lv > 30) && (lv % 2 == 0)) ||
		((lv <= 30) && (lv % 3 != 1)) || (randvit < 25)){
		vit++;
		gameLocal.Printf("+1 VIT!\n");
	}

	int randlck = rand() % 100;
	if (((lv < 40) && (lv > 31) && (lv % 2 == 1)) ||
		((lv <= 31) && (lv % 3 != 2)) ||
		(randlck < 25)){
		lck++;
		gameLocal.Printf("+1 LCK!\n");
	}

	hitp += 3;
	gameLocal.Printf("+3 HIT%!\n");

	md += 3;
	gameLocal.Printf("+3 MD!\n");
}

void CharacterFF::LevelUpWt(){
	int oldmaxhp = maxhp;
	maxhp += vit / 4;
	if ( ( (lv > 32) && ( lv % 5 == 2) ) ||
		 ( (lv <= 32) && (lv > 20) && ( lv % 4 == 0) ) ||
		 ( (lv <= 20) && (lv > 14) && ( lv % 3 == 2) ) ||
		 ( (lv <= 14) && ( lv % 2 == 0) ) ){
		maxhp += 23;
	}
	gameLocal.Printf("+%d HP!\n", maxhp - oldmaxhp);

	int randstr = rand() % 100;
	if (((lv > 4) && (lv % 3 == 1)) || 
		(lv <= 4) || (randstr < 25)){
		str++;
		gameLocal.Printf("+1 STR!\n");
	}

	int randagi = rand() % 100;
	if (((lv > 5) && (lv % 3 == 2)) || (randagi < 25) ||
		(lv == 2) || (lv == 3) || (lv == 5)){
		agi++;
		gameLocal.Printf("+1 AGI!\n");
	}

	int randintel = rand() % 100;
	if ((lv < 31) || (randintel < 25)) {
		intel++;
		gameLocal.Printf("+1 INT!\n");
	}

	int randvit = rand() % 100;
	if (((lv > 21) && (lv % 3 == 0)) ||
		((lv <= 21) && (lv % 2 == 0)) || (randvit < 25)){
		vit++;
		gameLocal.Printf("+1 VIT!\n");
	}

	int randlck = rand() % 100;
	if (((lv > 20) && (lv % 3 == 2)) ||
		((lv <= 20) && (lv % 2 == 0)) ||
		(randlck < 25)){
		lck++;
		gameLocal.Printf("+1 LCK!\n");
	}

	hitp += 1;
	gameLocal.Printf("+1 HIT%!\n");

	md += 2;
	gameLocal.Printf("+2 MD!\n");
}

void CharacterFF::LevelUpBl(){
	int oldmaxhp = maxhp;
	maxhp += vit / 4;
	if ((lv == 41) || (lv == 35) || (lv == 30) || (lv == 14) || (lv == 11) ||
		((lv <= 30) && (lv > 14) && (lv % 4 == 2)) ||
		((lv <= 8) && (lv % 2 == 0)) ){
		maxhp += 23;
	}
	gameLocal.Printf("+%d HP!\n", maxhp - oldmaxhp);

	int randstr = rand() % 100;
	if (((lv < 41) && (lv % 3 == 1)) || (randstr < 25)){
		str++;
		gameLocal.Printf("+1 STR!\n");
	}

	int randagi = rand() % 100;
	if (((lv < 40) && (lv % 3 == 0)) || (randagi < 25)){
		agi++;
		gameLocal.Printf("+1 AGI!\n");
	}

	intel++;
	gameLocal.Printf("+1 INT!\n");

	int randvit = rand() % 100;
	if (((lv > 12) && (lv % 4 == 0)) ||
		((lv <= 9) && (lv % 2 == 1)) || (randvit < 25)){
		vit++;
		gameLocal.Printf("+1 VIT!\n");
	}

	int randlck = rand() % 100;
	if (((lv > 10) && (lv % 4 == 2)) ||
		((lv <= 10) && (lv > 3) && (lv % 2 == 0)) ||
		(randlck < 25)){
		lck++;
		gameLocal.Printf("+1 LCK!\n");
	}

	hitp += 1;
	gameLocal.Printf("+1 HIT%!\n");

	md += 2;
	gameLocal.Printf("+2 MD!\n");
}