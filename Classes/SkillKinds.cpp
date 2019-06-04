#include "cocos2d.h"
#include "UnitClass.h"
#include "skillClass.h"
#include "GameScene.h"
#include "helper.h"
USING_NS_CC;
void UsingFireBall(GameScene *main_scene,Unit *hero,Vec2 pos) {
	static clock_t last_time = 0;
	clock_t now_time = clock();
	float pass_time = now_time - last_time;
	if (pass_time < 2000) {// cd时间在这里调整
		log("fireboll still cd");
		return;
	}
	last_time = now_time;
	auto skill = Skill::create("fireboll.jpg", 300, 10, 300, 50);
	skill->_skiller = hero;
	skill->setScale(0.3);
	skill->setPosition(hero->getPosition());
	skill->_st_pos = hero->getPosition();
	main_scene->skill_map[main_scene->skill_num] = skill;
	main_scene->skill_num++;
	skill->_side = 0;
	skill->_release_time = clock();
	auto map = hero->getParent();
	map->addChild(skill, 12);//这里有一点问题要解决
	skill->move(skill->_st_pos, pos);
}