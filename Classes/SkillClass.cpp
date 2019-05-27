#include "cocos2d.h"
#include "skillClass.h"
#include "GameScene.h"
#include <cocos-ext.h>
USING_NS_CC;
Skill* Skill::create(const std::string& filename,
	int speed,int damage) {

	auto unit = new (std::nothrow) Skill;
	if (unit && unit->initWithFile(filename)) {

		//��ʼ��unit��������
		unit->_speed = speed;
		unit->_damage = damage;
		unit->autorelease();
		return unit;
	}
	CC_SAFE_DELETE(unit);

	return nullptr;
}

bool GameScene::SkillHit(Skill *sk,Unit *un) {
	if (un->getPosition().getDistance(sk->getPosition()) < 50) {
		un->_life_current -= 10;
		log("dead!");
		return 1;
	}
	return 0;
}