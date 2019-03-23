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
	player->heroes[2] = CharacterFF("hoising", 90, FIGHTER);
	CharacterFF& c1 = player->heroes[0];
	CharacterFF& c2 = player->heroes[1];
	c3 = CharacterFF("chiang", 75, BL_MAGE);
	CharacterFF& c4 = player->heroes[2];

	//heroes[0] = c1;
	//heroes[1] = c2;
	//enemies[0] = c3;

	//setting vars to talk to gui file
	player->battleDisplay->SetStateInt("hero1_hp", c1.hp);
	player->battleDisplay->SetStateString("hero1_name", c1.name);
	player->battleDisplay->SetStateInt("hero2_hp", c2.hp);
	player->battleDisplay->SetStateString("hero2_name", c2.name);
	player->battleDisplay->SetStateInt("hero3_hp", c4.hp);
	player->battleDisplay->SetStateString("hero3_name", c4.name);
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
		player->hud->SetStateInt("current_hero", currentHero);
		player->hud->SetStateInt("next_state", 0);
		player->changePlayerHUD(player->hud, enemy);
	}
	else {
		gameLocal.Printf("The battle display was not found\n");
	}
}

void BattleManagerFF::PrepareCommand(const char* command){
	preparingCommand.Set("command", command);
	player->hud->SetStateString("message", "Choose your target");
}

void BattleManagerFF::AddCommand(const char* target) {
	preparingCommand.Set("target", target);
	preparingCommand.SetInt("attacker", currentHero);
	idDict cmd = preparingCommand;
	commandsQueue.push(cmd);
	preparingCommand.Clear();

	//get the next character
	//if the next character is not alive, skip it
	currentHero++;
	CharacterFF* nextHero = NULL;
	if (currentHero < 3) nextHero = &player->heroes[currentHero];

	while ( nextHero && nextHero->hp <= 0 && currentHero < 2 ) {
		currentHero++;
		nextHero = &player->heroes[currentHero];
	}

	player->hud->SetStateInt("current_hero", currentHero);
	gameLocal.Printf("The command is: %d will use %s on %s\n", cmd.GetInt("attacker"), cmd.GetString("command"), cmd.GetString("target"));
	if (!nextHero) {
		player->hud->SetStateInt("current_hero", 10);
		NextState();
	}
	//if (currentHero >= 3) NextState();
}

void BattleManagerFF::NextState(){
	gameLocal.Printf("Changing State\n");
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
	
	gameLocal.Printf("Performing queue\n");

	while (!commandsQueue.empty()) {
		idDict cmd = commandsQueue.first();
		commandsQueue.pop();
		const char* command = cmd.GetString("command");
		CharacterFF* h = &player->heroes[cmd.GetInt("attacker")];
		CharacterFF* target = GetEnt(cmd.GetString("target"));
		if (!target) {
			gameLocal.Printf("target not found\n");
			continue;
		}
		if (!h) {
			gameLocal.Printf("hero not found\n");
			continue;
		}
		if (!command) {
			gameLocal.Printf("command not found\n");
			continue;
		}

		gameLocal.Printf("%s will use %s on %s\n", h->name, command, target->name);
		if ( strcmp(command, "attack") == 0) {
			char* msg = h->Attack(target);
			gameLocal.Printf("%s\n", msg);
			messages.push(msg);
		}
	}

	commandsQueue.clear();
	//NextState();
	player->hud->SetStateInt("next_state", 2);
}

CharacterFF* BattleManagerFF::GetEnt(const char* ent) {
	gameLocal.Printf("looking for character %s\n", ent);
	//gameLocal.Printf("lets see if c3 is null at this point: %s\n", c3.name);
	
	if (strcmp(ent, "hero1") == 0) return &player->heroes[0];
	if (strcmp(ent, "hero2") == 0) return &player->heroes[1];
	if (strcmp(ent, "hero3") == 0) return &player->heroes[2];

	if (strcmp(ent, "ent1") == 0) { 
		//gameLocal.Printf("returning c3 address %d\n", &c3);
		return &c3; 
	}
	//if (ent == "ent1") return &player->heroes[0];
	//if (ent == "ent1") return &player->heroes[0];
	//if (ent == "ent1") return &player->heroes[0];
	//if (ent == "ent1") return &player->heroes[0];
	//if (ent == "ent1") return &player->heroes[0];

	return NULL;
}