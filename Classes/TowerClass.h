#ifndef TOWERCLASS
#define TOWERCLASS

#include "cocos2d.h"
#include "UnitClass.h"
USING_NS_CC;

class Tower : public Unit {
public:
	static Tower* create(const std::string& filename,
		int side
	);
	int hit_range;
	void fire(Unit* tar);
	clock_t _last_release_time;
	int _test = 0;
	clock_t _cd_time=2000;
	Unit* targe = NULL;
private:
	int _moveDir;
	float _attackInterval;
	Vec2 _pos_moveTarget;
	std::string _unitType;
	
};

#endif // !UNITCLASS