#include "../idlib/precompiled.h"
#pragma hdrstop

#include "BattleManagerFF.h"

void BattleManagerFF::SendBattleRequest(idActor* attacker, idActor* target) {
	gameLocal.Printf("Sending Battle Request\n");
	if (attacker->inBattle || target->inBattle) {
		gameLocal.Printf("one of them is already in battle\n");
		return;
	}
	if ( attacker->team == target->team || attacker == target)
	{
		gameLocal.Printf("same team\n");
		return;
	}
	if ( attacker->IsType(idAI::GetClassType()) && target->IsType(idPlayer::GetClassType())) {
		StartBattle(static_cast<idAI*>(attacker), static_cast<idPlayer*>(target));
		return;
	}
	if (attacker->IsType(idPlayer::GetClassType()) && target->IsType(idAI::GetClassType())) {
		StartBattle(static_cast<idAI*>(target), static_cast<idPlayer*>(attacker));
		return;
	}

	gameLocal.Printf("AI attacking AI or Player Attacking Player\n");
}

void BattleManagerFF::StartBattle(idAI* enemy, idPlayer* player) {
	this->enemy = enemy;
	this->player = player;
	//gui declarations
	//idUserInterface * hud = player->hud;
	//idUserInterface * defaultHUD = player->defaultHUD;
	//idUserInterface * battleDisplay = player->battleDisplay;
	
	//creating characters
	player->heroes[0] = CharacterFF("luis", 100, FIGHTER);
	player->heroes[1] = CharacterFF("alfredo", 50, WT_MAGE);
	CharacterFF& c1 = player->heroes[0];
	CharacterFF& c2 = player->heroes[1];
	c3 = CharacterFF("chiang", 75, BL_MAGE);

	//heroes[0] = c1;
	//heroes[1] = c2;
	//enemies[0] = c3;

	//setting vars to talk to gui file
	player->battleDisplay->SetStateInt("hero1_hp", c1.hp);
	player->battleDisplay->SetStateString("hero1_name", c1.name);
	player->battleDisplay->SetStateInt("hero2_hp", c2.hp);
	player->battleDisplay->SetStateString("hero2_name", c2.name);
	player->battleDisplay->SetStateInt("ent1_hp", c3.hp);
	player->battleDisplay->SetStateString("ent1_name", c3.name);
	
	//change hud to battle display and save default hud into a variable
	player->defaultHUD = player->hud;
	player->hud = player->battleDisplay;
	player->battleDisplayName = player->hud->Name();
	
	//player->defaultHUD = player->hud;
	//player->hud = player->battleDisplay;
	//gameLocal.Printf("raplacing %s with %s\nJust checking: %s\n", defaultHUD->Name(), hud->Name(), battleDisplay->Name());
	//battleDisplayName = player->hud->Name();
	if (player->hud) {
		state = P_SELECT;
		currentHero = 0;
		player->changePlayerHUD(player->hud, enemy);
	}
	else {
		gameLocal.Printf("The battle display was not found\n");
	}
}

void BattleManagerFF::PrepareCommand(const char* command){
	preparingCommand.Set("command", command);
}

void BattleManagerFF::AddCommand(const char* target) {
	preparingCommand.Set("target", target);
	preparingCommand.SetInt("attacker", currentHero);
	idDict cmd = preparingCommand;
	commandsQueue.push(cmd);
	preparingCommand.Clear();

	//do {
	//	if (currentHero < 2) currentHero++;
	//} while (player->heroes[currentHero].hp <= 0);

	gameLocal.Printf("The command is: %d will use %s on %s\n", cmd.GetInt("attacker"), cmd.GetString("command"), cmd.GetString("target"));

	if (currentHero >= 3) NextState();
}

void BattleManagerFF::NextState(){
	switch (state) {
		case P_SELECT:
			state = P_ATTACK;
			PerformQueue();
			break;
		case P_ATTACK:
			state = E_SELECT;
			break;
		case E_SELECT:
			state = E_ATTACK;
			break;
		case E_ATTACK:
			state = P_SELECT;
			break;
		default:
			state = P_SELECT;
			break;
	}
}

void BattleManagerFF::PerformQueue(){
	
	while (!commandsQueue.empty()) {
		idDict cmd = commandsQueue.last();
		commandsQueue.pop();
		const char* command = cmd.GetString("command");
		CharacterFF* h = &player->heroes[cmd.GetInt("attacker")];
		CharacterFF* target = GetEnt(cmd.GetString("target"));
		if ( command == "attack") {
			h->Attack(target);
		}
	}

	commandsQueue.clear();
	NextState();
}

CharacterFF* BattleManagerFF::GetEnt(const char* ent) {
	if (ent == "hero1") return &player->heroes[0];

	return NULL;
}