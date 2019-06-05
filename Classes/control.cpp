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
		if (hero->skill_statement == 0) {
			order od;
			od.kind = MOVEOERDER;
			od.pos = newPosition;
			od.tag = -1;
			hero->order_list.push_back(od);
		}
		if (hero->skill_statement == 1) {
			order od;
			od.kind = SKILLQ;
			od.pos = newPosition;
			od.tag = -1;//暂时先放着
			hero->order_list.push_back(od);
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