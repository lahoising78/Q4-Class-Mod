#include "../idlib/precompiled.h"
#pragma hdrstop

#include "CharacterFF.h"

CharacterFF::CharacterFF(const char* name, ClassType classType, int maxhp){
	exp = 0;
	gameLocal.Printf("the passed string is: %s\n", name);
	this->name = name;
	gameLocal.Printf("the new enemy is called %s\n", this->name);
	
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
		case IMP:
			CreateImp();
			break;
	}
	
	if (maxhp != 10) {
		this->maxhp = maxhp;
		hp = maxhp;
	}
}

void CharacterFF::CreateFighter(){
	maxhp = 35;
	hp = maxhp;
	str = 20;
	agi = 5;
	intel = 1;
	vit = 10;
	lck = 5;
	hitp = 10;
	md = 15;
}

void CharacterFF::CreateWtMage(){
	maxhp = 28;
	hp = maxhp;
	str = 5;
	agi = 5;
	intel = 15;
	vit = 10;
	lck = 5;
	hitp = 5;
	md = 20;
}

void CharacterFF::CreateBlMage() {
	maxhp = 25;
	hp = maxhp;
	str = 1;
	agi = 10;
	intel = 20;
	vit = 1;
	lck = 10;
	hitp = 5;
	md = 20;
}

void CharacterFF::CreateImp() {
	maxhp = hp = 8;
	str = 5;
	def = 4;
	md = 16;
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

//void CharacterFF::GainExperience(int n, rvQueue<const char*, 20> &messages) {
void CharacterFF::GainExperience(int n, rvQueue<idStr, 20> &messages) {

	//int gainedExp = n * 6;
	gameLocal.Printf("gaining %d experience\n", n);
	exp += n;
	int forNext = 40 << (lv - 1);
	gameLocal.Printf("exp: %d, for next lvl: %d\n", exp, forNext);
	bool newLvl = exp > forNext;
	if (newLvl){
		//idStr msg = name;
		lv++;
		gameLocal.Printf("Leveled up to lvl %d!!\n", lv);
		char msg[50];
		idStr::snPrintf(msg, sizeof(msg), "%s leveled up to lvl %d", name, lv);
		//msg += " leveld up to lvl ";
		//msg += lv;
		messages.push(msg);

		switch (classType)
		{
			case FIGHTER:
				LevelUpFighter(messages);
				break;
			case WT_MAGE:
				LevelUpWt(messages);
				break;
			case BL_MAGE:
				LevelUpBl(messages);
				break;
		}
	}
}

//void CharacterFF::LevelUpFighter(rvQueue<const char*, 20> &messages){
void CharacterFF::LevelUpFighter(rvQueue<idStr, 20> &messages){

	char msg[50];
	
	int oldmaxhp = maxhp;
	maxhp += vit / 4;
	if (((lv > 23) && (lv % 3 == 2)) || 
		((lv <= 23) && (lv > 11) && (lv % 3 != 0)) || 
		( lv <= 11)) {
		maxhp += 23;
	}
	gameLocal.Printf("+%d HP!\n", maxhp - oldmaxhp);
	idStr::snPrintf(msg, sizeof(msg), "%s MAX HP: %d", name, maxhp );
	messages.push(msg);

	str++;
	gameLocal.Printf("+1 STR!\n");
	idStr::snPrintf(msg, sizeof(msg), "%s STR UP!", name);
	messages.push(msg);

	int randagi = rand() % 100;
	if ( ((lv > 29) && (lv % 2 == 1)) || 
		 ((lv <= 29) && (lv > 20) && (lv % 3 != 0)) ||
		 (lv <= 20) || (randagi < 25) ){
		agi++;
		gameLocal.Printf("+1 AGI!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s AGI UP!", name);
		messages.push(msg);
	}

	int randintel = rand() % 100;
	if (((lv < 44) && (lv % 3 == 1)) || (randintel < 25)) {
		intel++;
		gameLocal.Printf("+1 INT!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s INT UP!", name);
		messages.push(msg);
	}

	int randvit = rand() % 100;
	if (((lv < 41) && (lv > 30) && (lv % 2 == 0)) ||
		((lv <= 30) && (lv % 3 != 1)) || (randvit < 25)){
		vit++;
		gameLocal.Printf("+1 VIT!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s VIT UP!", name);
		messages.push(msg);
	}

	int randlck = rand() % 100;
	if (((lv < 40) && (lv > 31) && (lv % 2 == 1)) ||
		((lv <= 31) && (lv % 3 != 2)) ||
		(randlck < 25)){
		lck++;
		gameLocal.Printf("+1 LCK!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s LCK UP!", name);
		messages.push(msg);
	}

	hitp += 3;
	gameLocal.Printf("+3 HIT%!\n");
	idStr::snPrintf(msg, sizeof(msg), "%s HITP +3!", name);
	messages.push(msg);

	md += 3;
	gameLocal.Printf("+3 MD!\n");
	idStr::snPrintf(msg, sizeof(msg), "%s MD +3!", name);
	messages.push(msg);
}

//void CharacterFF::LevelUpWt(rvQueue<const char*, 20> &messages){
void CharacterFF::LevelUpWt(rvQueue<idStr, 20> &messages){
	char msg[50];

	int oldmaxhp = maxhp;
	maxhp += vit / 4;
	if ( ( (lv > 32) && ( lv % 5 == 2) ) ||
		 ( (lv <= 32) && (lv > 20) && ( lv % 4 == 0) ) ||
		 ( (lv <= 20) && (lv > 14) && ( lv % 3 == 2) ) ||
		 ( (lv <= 14) && ( lv % 2 == 0) ) ){
		maxhp += 23;
	}
	gameLocal.Printf("+%d HP!\n", maxhp - oldmaxhp);
	idStr::snPrintf(msg, sizeof(msg), "%s MAX HP: %d", name, maxhp);
	messages.push(msg);

	int randstr = rand() % 100;
	if (((lv > 4) && (lv % 3 == 1)) || 
		(lv <= 4) || (randstr < 25)){
		str++;
		gameLocal.Printf("+1 STR!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s STR UP!", name);
		messages.push(msg);
	}

	int randagi = rand() % 100;
	if (((lv > 5) && (lv % 3 == 2)) || (randagi < 25) ||
		(lv == 2) || (lv == 3) || (lv == 5)){
		agi++;
		gameLocal.Printf("+1 AGI!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s AGI UP!", name);
		messages.push(msg);
	}

	int randintel = rand() % 100;
	if ((lv < 31) || (randintel < 25)) {
		intel++;
		gameLocal.Printf("+1 INT!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s INT UP!", name);
		messages.push(msg);
	}

	int randvit = rand() % 100;
	if (((lv > 21) && (lv % 3 == 0)) ||
		((lv <= 21) && (lv % 2 == 0)) || (randvit < 25)){
		vit++;
		gameLocal.Printf("+1 VIT!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s VIT UP!", name);
		messages.push(msg);
	}

	int randlck = rand() % 100;
	if (((lv > 20) && (lv % 3 == 2)) ||
		((lv <= 20) && (lv % 2 == 0)) ||
		(randlck < 25)){
		lck++;
		gameLocal.Printf("+1 LCK!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s LCK UP!", name);
		messages.push(msg);
	}

	hitp += 1;
	gameLocal.Printf("+1 HIT%!\n");
	idStr::snPrintf(msg, sizeof(msg), "%s HITP +1!", name);
	messages.push(msg);

	md += 2;
	gameLocal.Printf("+2 MD!\n");
	idStr::snPrintf(msg, sizeof(msg), "%s MD +2!", name);
	messages.push(msg);
}

//void CharacterFF::LevelUpBl(rvQueue<const char*, 20> &messages){
void CharacterFF::LevelUpBl(rvQueue<idStr, 20> &messages){
	char msg[50];

	int oldmaxhp = maxhp;
	maxhp += vit / 4;
	if ((lv == 41) || (lv == 35) || (lv == 30) || (lv == 14) || (lv == 11) ||
		((lv <= 30) && (lv > 14) && (lv % 4 == 2)) ||
		((lv <= 8) && (lv % 2 == 0)) ){
		maxhp += 23;
	}
	gameLocal.Printf("+%d HP!\n", maxhp - oldmaxhp);
	idStr::snPrintf(msg, sizeof(msg), "%s MAX HP: %d", name, maxhp);
	messages.push(msg);

	int randstr = rand() % 100;
	if (((lv < 41) && (lv % 3 == 1)) || (randstr < 25)){
		str++;
		gameLocal.Printf("+1 STR!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s STR UP!", name);
		messages.push(msg);
	}

	int randagi = rand() % 100;
	if (((lv < 40) && (lv % 3 == 0)) || (randagi < 25)){
		agi++;
		gameLocal.Printf("+1 AGI!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s AGI UP!", name);
		messages.push(msg);
	}

	intel++;
	gameLocal.Printf("+1 INT!\n");
	idStr::snPrintf(msg, sizeof(msg), "%s INT UP!", name);
	messages.push(msg);

	int randvit = rand() % 100;
	if (((lv > 12) && (lv % 4 == 0)) ||
		((lv <= 9) && (lv % 2 == 1)) || (randvit < 25)){
		vit++;
		gameLocal.Printf("+1 VIT!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s VIT UP!", name);
		messages.push(msg);
	}

	int randlck = rand() % 100;
	if (((lv > 10) && (lv % 4 == 2)) ||
		((lv <= 10) && (lv > 3) && (lv % 2 == 0)) ||
		(randlck < 25)){
		lck++;
		gameLocal.Printf("+1 LCK!\n");
		idStr::snPrintf(msg, sizeof(msg), "%s LCK UP!", name);
		messages.push(msg);
	}

	hitp += 1;
	gameLocal.Printf("+1 HIT%!\n");
	idStr::snPrintf(msg, sizeof(msg), "%s HITP +1!", name);
	messages.push(msg);

	md += 2;
	gameLocal.Printf("+2 MD!\n");
	idStr::snPrintf(msg, sizeof(msg), "%s MD +2!", name);
	messages.push(msg);
}

void CharacterFF::SetLevel(int lvl) {
	gameLocal.Printf("set level func called\n");
	if (lvl < 2) return;
	gameLocal.Printf("Setting lvl to %d", lvl);
	lv = lvl;
	maxhp += 15 * lvl;
	hp = maxhp;
	str += lvl + 3;
	def += lvl + 5;
	md += 2 * lvl;
}