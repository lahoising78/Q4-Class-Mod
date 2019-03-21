#ifndef	BATTLE_MANAGER_H
#define	BATTLE_MANAGER_H

class BattleManagerFF {
	public:
		void SendBattleRequest(idActor* attacker, idActor* target);
		void StartBattle(idAI* enemy, idPlayer* player);
};

#endif	/* Battle Manager */