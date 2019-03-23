
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
		CharacterFF(const char* name = "default name", int maxhp = 10, ClassType classType = FIGHTER);
		const char* Attack(CharacterFF* target);
		int lv;
		int exp;
		int hp;
		int maxhp;
		int mp;
		int str;
		int agl;
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
