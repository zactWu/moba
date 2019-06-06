#ifndef HEROCLOSE
#define HEROCLOSE

#include "cocos2d.h"
#include "UnitClass.h"
USING_NS_CC;

class Hero :public Unit {
public:
	static Hero* create(const std::string& filename, const std::string& heroType,
		int maxLife = 100, int attack = 20, int defense = 3, int speed = 100,
		float rotate_speed = 90.0f, float attackInterval = 2.f, float attackRange = 50.f);

	int _money;
	int _magic_current;
	int _magic_max;
	Vec2 reborn_pos = { 100.0,100.0 };
	//¼¼ÄÜÀ¸
	void UsingFireBall(Vec2 newPosition);
	void useSkill_trample();
	void useSkill_blinkToEnemy(Unit* enemy);
	void useSkill_ConeWave(const Vec2& pos_target);
	void useSkill_switchLife(Unit* enemy);
	clock_t Qskill_last_release_time;
	clock_t Wskill_last_release_time; 
	clock_t Eskill_last_release_time;
	clock_t Askill_last_release_time;
	clock_t Qskill_cd_time;
	clock_t Wskill_cd_time;
	clock_t Eskill_cd_time;
	clock_t Askill_cd_time;
	int skill_statement = 0;
};


#endif // !HERO
