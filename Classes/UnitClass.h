#ifndef UNITCLASS
#define UNITCLASS

#include "cocos2d.h"

USING_NS_CC;

enum Dir { down, down_right, right, up_right, up, up_left, left, down_left };

class Unit : public Sprite{
public:
	
	static Unit* create(const std::string &filename, const std::string &unitType,
		int maxLife = 300, int attack = 20, int defense = 3, int speed = 100, float rotate_speed = 90.0f);

	bool moveTo_directly(const Vec2 &pos_target);
	virtual void animate_move_forever(int dir);
	virtual void Unit::animate_move_forever_pos(Vec2 pos_target);
private:
	int _life_max;
	int _life_current;
	int _attack;
	int _defense;
	int _speed;
	int _moveDir;
	Vec2 pos_moveTarget;
	std::string _unitType;
};

#endif // !UNITCLASS