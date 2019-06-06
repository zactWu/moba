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
			
			od.kind = MOVEOERDER;
			od.pos = newPosition;
			
			hero->order_list.push_back(od);
		}
		if (hero->skill_statement == 1) {
			
			od.kind = SKILLQ;
			od.pos = newPosition;
			hero->order_list.push_back(od);
			hero->skill_statement = 0;
		}
		return true;
	};
	
	auto ketboard_listener = EventListenerKeyboard::create();
	ketboard_listener->onKeyPressed = [=](EventKeyboard::KeyCode keycode, Event* event) {
		if (hero->skill_statement == 0) {
			hero->skill_statement = 1;
			log("ready");
		}
		else {
			hero->skill_statement = 0;
			log("give up");
		}
	};
	auto ketboard_listener2 = EventListenerKeyboard::create();
	ketboard_listener2->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event* event) {
		log("sth released");
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouse_listener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(ketboard_listener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(ketboard_listener2, this);

}

int GameScene::ClickFindTag(Vec2 pos) {
	auto unit = unit_map.begin();
	float dis = 1000;
	int tag = -1;
	while (unit != unit_map.end()) {// 这一行是用来检查外部引用getdamage的指向性（放出技能的时候就知道能不能打中）技能的
		if (unit->second->_life_current <= 0) {
			
		}
		else {
			if (unit->second->getPosition().getDistance(pos) < 40 &&
				unit->second->getPosition().getDistance(pos) < dis) {

				

				dis = unit->second->getPosition().getDistance(pos);
				tag = unit->second->getTag();
			}
 			++unit;
		}
	}
	return tag;
}