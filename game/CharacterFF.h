
#ifndef CHARACTER_FF
#define CHARACTER_FF

// ==================mod======================
// Final Fantasy class types
enum ClassType {
	FIGHTER,
	BL_MAGE,
	WT_MAGE
};
//End of FF Class types
//===================end======================

class CharacterFF {
	public:
		CharacterFF(const char* name = "default name", ClassType classType = WT_MAGE, int maxhp = 10);
		void CreateFighter();
		//void LevelUpFighter(rvQueue<const char*, 20> &messages);
		void LevelUpFighter(rvQueue<idStr, 20> &messages);
		void CreateWtMage();
		//void LevelUpWt(rvQueue<const char*, 20> &messages);
		void LevelUpWt(rvQueue<idStr, 20> &messages);
		void CreateBlMage();
		//void LevelUpBl(rvQueue<const char*, 20> &messages);
		void LevelUpBl(rvQueue<idStr, 20> &messages);
		const char* Attack(CharacterFF* target);
		//void GainExperience(int n, rvQueue<const char*, 20> &messages);
		void GainExperience(int n, rvQueue<idStr, 20> &messages);
		int lv;
		int exp;
		int hp;
		int maxhp;
		int mp;
		int str;
		int agi;
		int intel;
		int vit;
		int lck;
		int atk;
		int def;
		int hitp;
		int evap;
		int md;
		
		const char* name;
		ClassType classType;
};

#endif	/* CHARACTER_FF */
