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
	
	//creating characters
	player->heroes[0] = CharacterFF("luis", 100, FIGHTER);
	player->heroes[1] = CharacterFF("alfredo", 50, WT_MAGE);
	player->heroes[2] = CharacterFF("hoising", 90, FIGHTER);
	CharacterFF& c1 = player->heroes[0];
	CharacterFF& c2 = player->heroes[1];
	CharacterFF& c4 = player->heroes[2];

	//setting vars to talk to gui file
	player->battleDisplay->SetStateInt("hero1_hp", c1.hp);
	player->battleDisplay->SetStateString("hero1_name", c1.name);
	player->battleDisplay->SetStateInt("hero2_hp", c2.hp);
	player->battleDisplay->SetStateString("hero2_name", c2.name);
	player->battleDisplay->SetStateInt("hero3_hp", c4.hp);
	player->battleDisplay->SetStateString("hero3_name", c4.name);
	PopulateEnemies();
	
	//change hud to battle display and save default hud into a variable
	player->defaultHUD = player->hud;
	player->hud = player->battleDisplay;
	player->battleDisplayName = player->hud->Name();
	
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

void BattleManagerFF::PopulateEnemies(){
	srand(time(0));
	int num = rand() % 6 + 1;
	for (int i = 1; i <= num; i++) {
		const char* name = "ent1_name";
		const char* hp = "ent1_hp";
		CharacterFF c = CharacterFF(name);
		enemies.Append( c );
		player->battleDisplay->SetStateInt(hp, c.hp);
		player->battleDisplay->SetStateString(name, c.name);
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
	//preparingCommand.Clear();

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
	//gameLocal.Printf("The command is: %d will use %s on %s\n", cmd.GetInt("attacker"), cmd.GetString("command"), cmd.GetString("target"));
	if (!nextHero) {
		player->hud->SetStateInt("current_hero", 10);
		NextState();
	}
	//if (currentHero >= 3) NextState();
}

void BattleManagerFF::NextState(){
	gameLocal.Printf("Changing State to: ");
	switch (state) {
		case P_SELECT:
			state = P_ATTACK;
			gameLocal.Printf("P_ATTACK %d\n", state);
			PerformQueue();
			break;
		case P_ATTACK:
			state = E_SELECT;
			gameLocal.Printf("E_SELECT %d\n", state);
			player->hud->SetStateInt("next_state", 6);
			EnemiesSelect();
			break;
		case E_SELECT:
			state = E_ATTACK;
			gameLocal.Printf("E_ATTACK %d\n", state);
			break;
		case E_ATTACK:
			state = P_SELECT;
			gameLocal.Printf("P_SELECT %d\n", state);
			player->hud->SetStateInt("next_state", 0);
			currentHero = 0;
			break;
		default:
			gameLocal.Printf("Default route? %d\n", state);
			state = P_SELECT;
			break;
	}
	//gameLocal.Printf("%d\n", state);
}

void BattleManagerFF::PerformQueue(){
	
	gameLocal.Printf("Performing queue\n");

	if (commandsQueue.empty()) {
		//commandsQueue.clear();
		NextState();
		return;
	}
	
	idDict cmd = commandsQueue.first();
	commandsQueue.pop();
	gameLocal.Printf("The command is: %d will use %s on %s\n", cmd.GetInt("attacker"), cmd.GetString("command"), cmd.GetString("target"));
	const char* command = cmd.GetString("command");
	CharacterFF* h;
	CharacterFF* target;
	if (state == P_ATTACK) {
		h = &player->heroes[cmd.GetInt("attacker")];
		target = GetEnt(cmd.GetString("target"));
	}
	else {
		h = &enemies[cmd.GetInt("attacker")];
		target = &enemies[cmd.GetInt("target")];
	}

	if (!target) {
		gameLocal.Printf("target not found\n");
		return; //continue;
	}
	if (!h) {
		gameLocal.Printf("hero not found\n");
		return;	//continue;
	}
	if (!command) {
		gameLocal.Printf("command not found\n");
		return;	// continue;
	}

	const char* msg;
	gameLocal.Printf("%s will use %s on %s\n", h->name, command, target->name);
	if ( strcmp(command, "attack") == 0) {
		msg = h->Attack(target);
		gameLocal.Printf("%s\n", msg);
	}
	else {
		msg = "not an attack";
	}
	
	player->hud->SetStateString("message", msg);
	player->hud->SetStateInt("next_state", 2);
}

CharacterFF* BattleManagerFF::GetEnt(const char* ent) {
	gameLocal.Printf("looking for character %s\n", ent);
	//gameLocal.Printf("lets see if c3 is null at this point: %s\n", c3.name);
	
	if (strcmp(ent, "hero1") == 0) return &player->heroes[0];
	if (strcmp(ent, "hero2") == 0) return &player->heroes[1];
	if (strcmp(ent, "hero3") == 0) return &player->heroes[2];

	if (strcmp(ent, "ent1") == 0) return &enemies[0]; 
	if (strcmp(ent, "ent2") == 0) return &enemies[1];
	if (strcmp(ent, "ent3") == 0) return &enemies[2];
	if (strcmp(ent, "ent4") == 0) return &enemies[3];
	if (strcmp(ent, "ent5") == 0) return &enemies[4];
	if (strcmp(ent, "ent6") == 0) return &enemies[5];

	return NULL;
}

void BattleManagerFF::EnemiesSelect(){
	gameLocal.Printf("Enemies selecting\n");	
	
	commandsQueue.clear();
	idDict cmd;
	cmd.Set("command", "attack");
	cmd.SetInt("attacker", 0);
	int t = 0;
	cmd.SetInt("target", t);
	commandsQueue.push(cmd);
	//cmd.Clear();

	NextState();
}