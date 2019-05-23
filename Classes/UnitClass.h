#ifndef UNITCLASS
#define UNITCLASS

#include "cocos2d.h"

USING_NS_CC;

enum Dir { down, down_right, right, up_right, up, up_left, left, down_left };
enum Tag { move = 2001, animate_move };

class Unit : public Sprite {
public:

	static Unit* create(const std::string& filename, const std::string& unitType,
		int maxLife = 300, int attack = 20, int defense = 3, int speed = 100,
		float rotate_speed = 90.0f, float attackInterval = 1.f);

	void moveTo_directly(const Vec2& pos_target);
	void moveTo_directly(const std::vector<Vec2> pos_list);
	void attack_once(int iDir, int tag_enemy);
	virtual void animate_move_forever(int dir);

	int getDirByTargetPos(const Vec2& pos_target)const;
	int getDirByTargetPos(const Vec2& pos_current, const Vec2& pos_target)const;
	int getMoveDir() { return _moveDir; }

	int _life_current;
	int _test = 0;
private:
	bool _onAttack;
	int _life_max;
	int _attack;
	int _defense;
	int _speed;
	int _moveDir;
	float _attackInterval;

	Vec2 _pos_moveTarget;
	std::string _unitType;
};

#endif // !UNITCLASS