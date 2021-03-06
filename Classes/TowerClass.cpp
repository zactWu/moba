#include "cocos2d.h"
#include "skillClass.h"
#include "GameScene.h"
#include <cocos-ext.h>
#include "TowerClass.h"
USING_NS_CC;
Tower* Tower::create(const std::string& filename,
	int side
	) {

	auto unit = new (std::nothrow) Tower;
	if (unit && unit->initWithFile(filename)) {
		//初始化各项属性,这里的unit方式没有管，要改数值这里改就好了
		unit->_defense = 20;
		unit->_attack = 100;
		unit->hit_range = 300;
		unit->_side = side;
		unit->_last_release_time = 0;
		unit->_kill_award = 300;
		unit->_last_attacker = NULL;
		unit->_life_current = 2000;

		unit->_lifeBank = Sprite::create("lifeBank.jpg");
		unit->_lifeBank->setPosition(unit->getPosition() + Vec2(60.f, 120.f));
		unit->addChild(unit->_lifeBank, 100);
		unit->autorelease();
		return unit;
	}
	CC_SAFE_DELETE(unit);
	return nullptr;
}

void Tower::fire(Unit *tar) {
	_last_release_time = clock();
	auto skill = Skill::create("fireball.png", 300, 30, 300, 50);
	skill->_skiller = this;
	skill->setScale(0.25);
	skill->setPosition(this->getPosition());
	skill->_st_pos = this->getPosition();
	auto main_scene = dynamic_cast<GameScene*>(getParent()->getParent());
	main_scene->skill_map[main_scene->skill_num] = skill;
	main_scene->skill_num++;
	skill->_side = this->_side-2;
	skill->_release_time = clock();
	skill->targe = tar;
	auto map = this->getParent();
	map->addChild(skill);//这里有一点问题要解决
	skill->move(skill->_st_pos, tar->getPosition());
	return;
}
