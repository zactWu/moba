#ifndef SKILLCLASS
#define SKILLCLASS

#include "cocos2d.h"

USING_NS_CC;

class Skill : public Sprite {
public:

	static Skill* create(const std::string& filename,
		int speed,
		int damage
		);
	int _speed;
	int _damage;
	clock_t _release_time;
	Vec2 _st_pos;
	int _test = 0;
private:
	int _moveDir;
	float _attackInterval;

	Vec2 _pos_moveTarget;
	std::string _unitType;
};

#endif // !UNITCLASS