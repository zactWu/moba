#ifndef UNITCLASS
#define UNITCLASS

#include "cocos2d.h"
#include "GlobalVal.h"
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


	int _attack = 1;
	int _defense = 1;
	int _speed = 1;
	int _moveDir = 1;
	float _attackInterval = 1;
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
	int _side = this_computer_side;
	int _life_current = 1;
	int _tag_attackTarget=-1;
	int _kill_award = 1;
	int _kind = 0;
	float _attackRange = 1;
	Unit* _last_attacker=NULL;
	cocos2d::Sprite* _lifeBank;
	void stunned(double duration);

	void getDamaged();

	void GetOrder();

	bool _stunned=0;
	int _it_tag = -1;

	bool wound = 0;

	void Unit::longRangeAttack(Unit* enemy);
	// �������Ƿ������õ�
	
	std::vector<order> order_list;
	int _life_max = 100;
	bool _onAttack = 0;

	//��������������ϵͳ
	int exp = 300;
	int level = 1;

	Vec2 _pos_moveTarget;
	std::string _unitType;
};

#endif // !UNITCLASS