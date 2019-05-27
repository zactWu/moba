#include "cocos2d.h"
#include "skillClass.h"
#include "GameScene.h"
#include <cocos-ext.h>
USING_NS_CC;
Skill* Skill::create(const std::string& filename,
	int speed,int damage,int moverange,int hitrange) {

	auto unit = new (std::nothrow) Skill;
	if (unit && unit->initWithFile(filename)) {

		//初始化unit各项属性
		unit->_speed = speed;
		unit->_damage = damage;
		unit->move_range = moverange;
		unit->hit_range = hitrange;
		unit->autorelease();
		return unit;
	}
	CC_SAFE_DELETE(unit);

	return nullptr;
}

bool GameScene::SkillHit(Skill *sk,Unit *un) {
	if (sk->_side == un->_side) {
		log("unit side is %d", sk ->_side);
		return 0;
	}
	if (un->getPosition().getDistance(sk->getPosition()) < sk->hit_range) {
		float dis = un->getPosition().getDistance(sk->getPosition());
		log("dis is %f", dis);
		un->_life_current -= 10;
		//log("dead!");
		return 1;
	}
	return 0;
}
void Skill::move(Vec2 from, Vec2 to) {
	float times = move_range / from.getDistance(to);
	Vec2 should;
	should.x = (to.x - from.x) * times + from.x;
	should.y = (to.y - from.y) * times + from.y;
	auto move = MoveTo::create(move_range / _speed, should);
	this->runAction(move);
	return;
}