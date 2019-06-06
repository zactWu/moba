#ifndef UNITCLASS
#define UNITCLASS

#include "cocos2d.h"

USING_NS_CC;

enum Dir { down, down_right, right, up_right, up, up_left, left, down_left };
enum Tag { move = 2001, animate_move };
struct  order//服务器用的，不用管
{
	Vec2 pos;
	int kind = 0;
	int tag = -1;
};

class Unit : public Sprite {
public:

	static Unit* create(const std::string& filename, const std::string& unitType,
		int maxLife = 100, int attack = 20, int defense = 3, int speed = 100,
		float rotate_speed = 90.0f, float attackInterval = 2.f, float attackRange = 50.f);

	void moveTo_directly(const Vec2& pos_target);
	void moveTo_directly(const std::vector<Vec2> pos_list);
	void attack_once(Unit* sp_enemy);
	void update_follow_attack(float dt);
	void getDamaged(Unit* producer, int damage);
	//Unit* create(const std::string& filename, const std::string& unitType, GameScene* where, int maxLife, int attack, int defense, int speed, float rotate_speed, float attackInterval, float attackRange);
	//Unit* create(const std::string& filename, const std::string& unitType, int maxLife, int attack, int defense, int speed, float rotate_speed, float attackInterval, float attackRange, GameScene* where);
	virtual void animate_move_forever(int dir);

	int getDirByTargetPos(const Vec2& pos_target)const;
	int getDirByTargetPos(const Vec2& pos_current, const Vec2& pos_target)const;
	int getMoveDir() { return _moveDir; }
	int _money;
	int _side;
	int _life_current;
	int _tag_attackTarget=-1;
	int _kill_award;
	int _life_max;
	Unit* _last_attacker=NULL;
	cocos2d::Sprite* _lifeBank;
	void stunned(double duration);

	bool _stunned=0;

	int _it_tag = -1;

	void Unit::longRangeAttack(Unit* enemy);
	// 接下来是服务器用的
	
	std::vector<order> order_list;

protected:
	bool _onAttack;
	int _attack;
	int _defense;
	int _speed;
	int _moveDir;
	float _attackInterval;
	float _attackRange;
	Vec2 _pos_moveTarget;
	std::string _unitType;
};

#endif // !UNITCLASS