#ifndef	BATTLE_MANAGER_H
#define	BATTLE_MANAGER_H

#include "CharacterFF.h"
#include "AbilityFF.h"

enum BattleStates {
	P_SELECT,
	P_ATTACK,
	E_SELECT,
	E_ATTACK,
	P_WON,
	P_DOWN
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
		void Rewind();
		CharacterFF* ChooseNextHero();
		void EnemiesSelect();
		void UpdateHealth();
		void Victory();
		void EndBattle();
		CharacterFF* GetEnt(const char* ent);
		AbilityFF* GetMagic(const char* ab);
		
		BattleStates state;
		idAI* enemy;
		idPlayer* player;
		rvQueue<idDict, 6> commandsQueue;
		rvQueue<idStr, 20> messages;

		idDict preparingCommand;
		int currentHero = 0;

		idList<CharacterFF> enemies;
		AbilityFF cure = AbilityFF(CURE);
};

#endif	/* Battle Manager */