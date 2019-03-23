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
		void PopulateEnemies();
		void PrepareCommand(const char* command);
		void AddCommand(const char* target);
		void NextState();
		void PerformQueue();
		void LoadNextMessage();
		void EnemiesSelect();
		CharacterFF* GetEnt(const char* ent);
		
		BattleStates state;
		idAI* enemy;
		idPlayer* player;
		rvQueue<idDict, 6> commandsQueue;
		//idDict heroes;
		//idDict enemies;
		idDict preparingCommand;
		int currentHero = 0;

		idList<CharacterFF> enemies;
};

#endif	/* Battle Manager */