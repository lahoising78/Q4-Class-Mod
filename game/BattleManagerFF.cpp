#include "../idlib/precompiled.h"
#pragma hdrstop

#include "BattleManagerFF.h"
#include "CharacterFF.h"
#include "Player.h"

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
	//gui declarations
	//idUserInterface * hud = player->hud;
	//idUserInterface * defaultHUD = player->defaultHUD;
	//idUserInterface * battleDisplay = player->battleDisplay;
	
	//creating characters
	CharacterFF c1 = CharacterFF("luis", 100);
	CharacterFF c2 = CharacterFF("alfredo", 50);
	CharacterFF c3 = CharacterFF("chiang", 75);

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
		player->changePlayerHUD(player->hud, enemy);
		//gameLocal.sessionCommand = "game_startmenu";
	}
	else {
		gameLocal.Printf("The battle display was not found\n");
	}
}