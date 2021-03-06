#ifndef SKILLCLASS
#define SKILLCLASS

#include "cocos2d.h"
#include "UnitClass.h"
USING_NS_CC;

class Skill : public Sprite {
public:
	static Skill* create(const std::string& filename,
		int speed,
		int damage,
		int move_range,
		int hit_range
		);
	int _speed;
	int _damage;
	int _range;
	int move_range;
	int hit_range;
	int _side;
	clock_t _release_time;
	Vec2 _st_pos;
	int _test = 0;
	Unit* _skiller;
	Unit* targe = NULL;
	void move(Vec2 from, Vec2 to);
	
private:
	int _moveDir;
	float _attackInterval;
	Vec2 _pos_moveTarget;
	std::string _unitType;

};

#endif // !UNITCLASS