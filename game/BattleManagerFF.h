#ifndef	BATTLE_MANAGER_H
#define	BATTLE_MANAGER_H

#include "CharacterFF.h"

enum BattleStates {
	P_SELECT,
	P_ATTACK,
	E_SELECT,
	E_ATTACK
};

class BattleManagerFF {
	public:
		void SendBattleRequest(idActor* attacker, idActor* target);
		void StartBattle(idAI* enemy, idPlayer* player);
		void PrepareCommand(const char* command);
		void AddCommand(const char* target);
		void NextState();
		void PerformQueue();
		CharacterFF* GetEnt(const char* ent);
		
		BattleStates state;
		idAI* enemy;
		idPlayer* player;
		//CharacterFF h;
		rvQueue<idDict, 6> commandsQueue;
		idDict heroes;
		idDict enemies;
		idDict preparingCommand;
		int currentHero = 0;

		//to be deleted
		CharacterFF c3;
};

#endif	/* Battle Manager */