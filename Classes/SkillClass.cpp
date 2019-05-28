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

void GameScene::UsingFireBoll(Unit *hero,Vec2 newPosition) {
	static clock_t last_time = 0;
	clock_t now_time = clock();
	float pass_time = now_time - last_time;
	if (pass_time < 2000) {// cd时间在这里调整
		log("fireboll still cd");
		return;
	}
	last_time = now_time;
	auto skill = Skill::create("fireboll.jpg", 300, 10, 300, 50);
	skill->setScale(0.3);
	skill->setPosition(hero->getPosition());
	skill->_st_pos = hero->getPosition();
	skill_map[skill_num] = skill;
	skill_num++;
	skill->_side = 0;
	skill->_release_time = clock();
	map->addChild(skill, 12);//这里有一点问题要解决
	skill->move(skill->_st_pos, newPosition);
}