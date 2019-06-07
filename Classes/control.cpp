#include "cocos2d.h"
#include "UnitClass.h"
#include "GameScene.h"
#include "control.h"
USING_NS_CC;

void GameScene::ListenOutside() {
	auto mouse_listener = EventListenerTouchOneByOne::create();
	mouse_listener->onTouchBegan = [=](Touch* Touch, Event* Event) {
		auto touchPosition = Touch->getLocation();
		auto mapPosition = map->getPosition();
		auto nowPosition = hero->getPosition();
		auto newPosition = touchPosition - mapPosition;
		log("heard a check %f %f", newPosition.x, newPosition.y);
		log("%d", ClickFindTag(newPosition));
		
		order od;
		od.tag = ClickFindTag(newPosition);
		if (hero->skill_statement == 0) {
			hero->_tag_attackTarget = od.tag;
			
			log("this tag is %d", od.tag);
			od.kind = MOVEOERDER;
			od.pos = newPosition;
			
		}
		else if (hero->skill_statement!=0) {
			hero->_tag_attackTarget = -1;
			od.kind = hero->skill_statement;
			od.pos = newPosition;
			log("skill %d release", hero->skill_statement);
			hero->skill_statement = 0;
		}
		hero->order_list.push_back(od);
		return true;
	};
	
	auto ketboard_listener = EventListenerKeyboard::create();
	ketboard_listener->onKeyPressed = [=](EventKeyboard::KeyCode keycode, Event* event) {
		if (keycode == EventKeyboard::KeyCode::KEY_Q) {
			if (hero->skill_statement !=2) {
				hero->skill_statement = 2;
				log("Q ready");
			}
			else {
				hero->skill_statement = 0;
				log("give up");
			}
		}
		if (keycode == EventKeyboard::KeyCode::KEY_W) {
			if (hero->skill_statement != 3) {
				hero->skill_statement = 3;
				log("W ready");
			}
			else {
				hero->skill_statement = 0;
				log("give up");
			}
		}
		if (keycode == EventKeyboard::KeyCode::KEY_E) {
			if (hero->skill_statement !=4) {
				hero->skill_statement = 4;
				log("W ready");
			}
			else {
				hero->skill_statement = 0;
				log("give up");
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouse_listener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(ketboard_listener, this);

}

int GameScene::ClickFindTag(Vec2 pos) {
	auto unit = unit_map.begin();
	float dis = 1000;
	int tag = -1;
	int side = hero->_side;
	while (unit != unit_map.end()) {// 这一行是用来检查外部引用getdamage的指向性（放出技能的时候就知道能不能打中）技能的
		if (unit->second->_life_current <= 0) {
			
		}
		else {
			if (unit->second->getPosition().getDistance(pos) < 40 &&
				unit->second->getPosition().getDistance(pos) < dis &&
				unit->second->_side!=side) {

				dis = unit->second->getPosition().getDistance(pos);
				tag = unit->second->getTag();
			}
 			++unit;
		}
	}
	auto tower = tower_map.begin();
	while (tower != tower_map.end()) {// 这一行是用来检查外部引用getdamage的指向性（放出技能的时候就知道能不能打中）技能的
		if (tower->second->_life_current <= 0) {
			
		}
		else {
			if (tower->second->getPosition().getDistance(pos) < 40 &&
				tower->second->getPosition().getDistance(pos) < dis &&
				tower->second->_side-2 != side) {

				log("tower size %d and tag is %d",tower_map.size(),tower->second->_it_tag);
				dis = tower->second->getPosition().getDistance(pos);
				tag = tower->second->getTag();
			}
			++tower;
		}
	}
	return tag;
}