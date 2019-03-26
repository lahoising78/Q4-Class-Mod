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
	
	gameLocal.Printf("Starting Battle\n");
	//creating characters
	//player->heroes[0] = CharacterFF("luis", FIGHTER);
	//player->heroes[1] = CharacterFF("alfredo", WT_MAGE);
	//player->heroes[2] = CharacterFF("hoising", BL_MAGE);
	CharacterFF& c1 = player->heroes[0];
	CharacterFF& c2 = player->heroes[1];
	CharacterFF& c4 = player->heroes[2];

	//setting vars to talk to gui file
	player->battleDisplay = uiManager->FindGui("guis/turnbasedbattle.gui", true, false, true);
	player->battleDisplay->InitFromFile("guis/turnbasedbattle.gui");
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
		if (player->heroes[currentHero].hp <= 0) ChooseNextHero();
		player->hud->SetStateInt("current_hero", currentHero);
		player->hud->SetStateInt("next_state", 0);
		player->changePlayerHUD(player->hud, enemy);
	}
	else {
		gameLocal.Printf("The battle display was not found\n");
	}
}

void BattleManagerFF::PopulateEnemies(){
	gameLocal.Printf("Populating Enemies\n");
	srand(time(0));
	enemies.Clear();

	int avglvl = 0;
	for (int i = 0; i < 3; i++) {
		avglvl += player->heroes[i].lv;
	}
	avglvl = avglvl/3;
	gameLocal.Printf("%d\n", avglvl);

	int num = rand() % 6 + 1;
	//int num = 2; //temporary for debug
	for (int i = 1; i <= num; i++) {
		CharacterFF c = CharacterFF("Enemy");
		c.SetLevel(avglvl);
		enemies.Append( c );
		idStr hp = "ent";
		hp += i;
		hp += "_hp";
		player->battleDisplay->SetStateInt(hp.c_str(), c.hp);
		idStr name = "ent";
		name += i;
		name += "_name";
		player->battleDisplay->SetStateString(name.c_str(), c.name);
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

	//get the next character
	//if the next character is not alive, skip it
	/*currentHero++;
	CharacterFF* nextHero = NULL;
	if (currentHero < 3) nextHero = &player->heroes[currentHero];

	while ( nextHero && nextHero->hp <= 0 && currentHero < 2 ) {
		currentHero++;
		nextHero = &player->heroes[currentHero];
	} */
	CharacterFF* nextHero = ChooseNextHero();

	player->hud->SetStateInt("current_hero", currentHero);
	if (!nextHero) {
		player->hud->SetStateInt("current_hero", 10);
		NextState();
	}
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
			if (player->heroes[currentHero].hp <= 0) ChooseNextHero();
			player->hud->SetStateInt("current_hero", currentHero);
			break;
		case P_WON:
			gameLocal.Printf("P_SELECT %d\n", state);
			Victory();
			break;
		default:
			gameLocal.Printf("Default route? %d\n", state);
			state = P_SELECT;
			break;
	}
}

void BattleManagerFF::PerformQueue(){
	
	gameLocal.Printf("Performing queue\n");

	if (state == P_WON) {
		if (!messages.empty()) {
			const char* msg = messages.first();
			messages.pop();
			player->hud->SetStateString("message", msg);
		}
		else {
			//player->hud->SetStateInt("next_state", 6);
			player->hud->SetStateInt("next_state", 0);
			EndBattle();
		}
		return;
	}

	if (commandsQueue.empty()) {
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
		target = &player->heroes[cmd.GetInt("target")];
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
	else if (strcmp(command, "magic1") == 0) {
		gameLocal.Printf("should use magic\n");
		AbilityFF* ab = GetMagic(h->abilities[0]);
		if (ab) ab->Perform(h, target);
	}
	else {
		msg = "not an attack";
	}
	
	player->hud->SetStateString("message", msg);
	player->hud->SetStateInt("next_state", 2);
	UpdateHealth();
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

AbilityFF* BattleManagerFF::GetMagic(const char* ab) {
	gameLocal.Printf("looking for %s\n", ab);
	if (strcmp(ab, "CURE") == 0) {;
		return &cure;
	}
	return NULL;
}

void BattleManagerFF::EnemiesSelect(){
	gameLocal.Printf("Enemies selecting\n");	
	
	commandsQueue.clear();

	for (int i = 0; i < enemies.Num(); i++) {
		if (enemies[i].hp <= 0) continue;
		idDict cmd;
		cmd.Set("command", "attack");
		cmd.SetInt("attacker", i);
		int t = rand() % 3;
		while (player->heroes[t].hp <= 0){
			t = rand() % 3;
		}
		cmd.SetInt("target", t);
		commandsQueue.push(cmd);
	}

	NextState();
}

void BattleManagerFF::UpdateHealth(){
	player->hud->SetStateInt("hero1_hp", player->heroes[0].hp);
	player->hud->SetStateInt("hero2_hp", player->heroes[1].hp);
	player->hud->SetStateInt("hero3_hp", player->heroes[2].hp);

	int num = enemies.Num();
	player->hud->SetStateInt("ent1_hp", enemies[0].hp);
	if (num >= 2) player->hud->SetStateInt("ent2_hp", enemies[1].hp);
	if (num >= 3) player->hud->SetStateInt("ent3_hp", enemies[2].hp);
	if (num >= 4) player->hud->SetStateInt("ent4_hp", enemies[3].hp);
	if (num >= 5) player->hud->SetStateInt("ent5_hp", enemies[4].hp);
	if (num >= 6) player->hud->SetStateInt("ent6_hp", enemies[5].hp);

	bool down = true;
	for (int i = 0; i < 3; i++) {
		if (player->heroes[i].hp > 0) {
			down = false;
			break;
		}
		else down = true;
	}
	if (down) {
		gameLocal.Printf("Losing!!!");
		EndBattle();
		player->health = 0;
		return;
	}

	for (int i = 0; i < num; i++) {
		if (enemies[i].hp > 0) {
			down = false;
			break;
		}
		else down = true;
	}
	if (down) {
		gameLocal.Printf("You won!\n");
		state = P_WON;
		NextState();
	}

}

void BattleManagerFF::Rewind(){
	if (currentHero > 0) currentHero--;
	player->hud->SetStateInt("current_hero", currentHero);
	preparingCommand.Clear();
	commandsQueue.pop();
}

void BattleManagerFF::Victory(){
	enemy->health = 0;
	enemy->SetState("State_Killed");

	int num = enemies.Num();
	int lvl = enemies[0].lv;
	gameLocal.Printf("clearing enemy list\n");
	enemies.Clear();

	int membersAlive = 0;
	for (int i = 0; i < 3; i++) {
		if (player->heroes[i].hp > 0) membersAlive++;
	}

	int exp = num * lvl * 6 / membersAlive;
	for (int i = 0; i < 3; i++){
		CharacterFF &h = player->heroes[i];
		if (h.hp > 0){
			h.GainExperience(exp, messages);
		}
	}
}

void BattleManagerFF::EndBattle(){
	enemy->inBattle = false;
	enemies.Clear();
	messages.clear();
	commandsQueue.clear();
	preparingCommand.Clear();
	currentHero = 0;

	player->hud = player->defaultHUD;
	player->hud->ClearState();
	player->ClearFocus();
	//player->battleDisplay->ClearState();
	player->battleDisplay = NULL;
	player->inBattle = false;
}

CharacterFF* BattleManagerFF::ChooseNextHero(){
	currentHero++;
	CharacterFF* nextHero = NULL;
	if (currentHero < 3) nextHero = &player->heroes[currentHero];

	while (nextHero && nextHero->hp <= 0 && currentHero < 2) {
		currentHero++;
		nextHero = &player->heroes[currentHero];
	} 

	if (nextHero && nextHero->hp <= 0) nextHero = NULL;

	idUserInterface * hud = player->hud;
	hud->DeleteState("ab1");
	hud->DeleteState("ab2");
	hud->DeleteState("ab3");
	hud->DeleteState("ab4");
	hud->DeleteState("ab5");
	hud->DeleteState("ab6");

	if (nextHero) {
		for (int i = 0; i < nextHero->abilities.Num(); i++) {
			char label[4];
			idStr::snPrintf(label, sizeof(label), "ab%d", i+1);
			gameLocal.Printf(label);
			hud->SetStateString(label, nextHero->abilities[i]);
		}
		hud->SetStateInt("num_abs", nextHero->abilities.Num());
	}

	return nextHero;
}