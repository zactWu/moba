#ifndef UNITCLASS
#define UNITCLASS

#include "cocos2d.h"

USING_NS_CC;

enum Dir { down, down_right, right, up_right, up, up_left, left, down_left };
enum Tag { move = 2001, animate_move };
struct  order
{
	Vec2 pos;
	int kind = 0;
	int tag = -1;
};

class Unit : public Sprite {
public:


	int _attack;
	int _defense;
	int _speed;
	int _moveDir;
	float _attackInterval;
	int _kill_times = 0;
	int _dead_times = 0;

	static Unit* create(const std::string& filename, const std::string& unitType,
		int maxLife = 100, int attack = 5, int defense = 3, int speed = 100,
		float rotate_speed = 90.0f, float attackInterval = 0.3f, float attackRange = 50.f);

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
	int _money=0;
	int _side;
	int _life_current;
	int _tag_attackTarget=-1;
	int _kill_award;
	int _kind;
	float _attackRange;
	Unit* _last_attacker=NULL;
	cocos2d::Sprite* _lifeBank;
	void stunned(double duration);

	void getDamaged();

	void GetOrder();

	bool _stunned=0;
	int _it_tag = -1;

	bool wound = 0;

	void Unit::longRangeAttack(Unit* enemy);
	// 接下来是服务器用的
	
	std::vector<order> order_list;
	int _life_max;
	bool _onAttack;

	//接下来就是升级系统


	Vec2 _pos_moveTarget;
	std::string _unitType;
};

#endif // !UNITCLASS