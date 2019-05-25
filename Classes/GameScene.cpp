#include "GameScene.h"
#include "cocos2d.h"
#include <windows.h>
#include "MoveFind.h"

int num = 1;	//num用来记录场景中已经加入了多少unit，用于tag标签及unitMap

cocos2d::Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init() {
	if (!Layer::init())
	{
		return false;
	}
	MapInit();			//map初始化
	HeroInit();
	PointInit();
	this->schedule(schedule_selector(GameScene::AllActionsTakenEachF));		//设置一个update，每一帧都调用，做各种检测
	this->schedule(schedule_selector(GameScene::AddSoldiers), 5.0f);		//五秒出一波兵
	this->schedule(schedule_selector(GameScene::AttackSearch));
	//鼠标监听
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch * Touch, Event * Event) {
		auto touchPosition = Touch->getLocation();
		auto mapPosition = map->getPosition();
		auto nowPosition = hero->getPosition();
		auto newPosition = touchPosition - mapPosition;
		std::vector<Vec2> route = MoveFind(hero->getPosition(), newPosition);
		hero->moveTo_directly(route);
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

bool GameScene::MapInit()
{
	map = TMXTiledMap::create("map.tmx");		//创建map
	map->setAnchorPoint(Vec2(0, 0));			//设置锚点为左下角
	map->setPosition(Vec2(0, 0));				//设置位置为窗口左下角
	this->addChild(map);

	viewSize = Director::getInstance()->getOpenGLView()->getVisibleRect().size;		//初始化窗口大小
	mapSize = map->getMapSize();													//初始化map大小（单位：块）
	tileSize = map->getTileSize();													//初始化瓦片大小（单位：像素）

	//初始化map移动的四个边界，map移动时不可以超出这个边界
	mapMoveRight_x = 0;
	mapMoveLeft_x = viewSize.width - map->getMapSize().width * map->getTileSize().width;
	mapMoveUp_y = 0;
	mapMoveDowm_y = viewSize.height - map->getMapSize().height * map->getTileSize().height;

	
	return true;
}

bool GameScene::HeroInit()
{
	hero = Unit::create("soldier/0.png", "soldier");
	map->addChild(hero, 1, num);
	unitMap[num] = hero;			//map中英雄
	num++;
	return true;
}



void GameScene::AllActionsTakenEachF(float dt)
{
	int RectWidth = viewSize.width;		//窗口宽
	int RectHeight = viewSize.height;	//窗口高
	//以下几行获取鼠标位于窗口的坐标
	POINT p;
	GetCursorPos(&p);
	HWND hwnd = GetActiveWindow();
	ScreenToClient(hwnd, &p);
	p.y = RectHeight - p.y;

	//50是我设定的区域，鼠标位于该区域内则会导致map移动，50可更改
	if (RectWidth - p.x < 50 || RectHeight - p.y < 50 || p.x < 50 || p.y < 50) {
		//下面两行的 10 是自定义设置的，即每一设定的时间（目前设定一帧所需要的时间）移动10个像素，可更改，另外，sqrt用于开根号并计算两点距离，感觉式子有点low.....要是有办法写得高大上一点是最好
		auto x = 10 * (RectWidth / 2 - p.x) / sqrt((RectWidth / 2 - p.x) * (RectWidth / 2 - p.x) + (RectHeight / 2 - p.y) * (RectHeight / 2 - p.y));
		auto y = 10 * (RectHeight / 2 - p.y) / sqrt((RectHeight / 2 - p.y) * (RectHeight / 2 - p.y) + (RectWidth / 2 - p.x) * (RectWidth / 2 - p.x));
		//先计算一下移动后的new_x,new_y坐标
		auto new_x = map->getPosition().x + x;
		auto new_y = map->getPosition().y + y;
		//若是移动后的new_x,new_y坐标会导致map超出窗口，则更改x,y坐标
		if (new_x < mapMoveLeft_x) {
			x = mapMoveLeft_x - map->getPosition().x;
		}
		if (new_x > mapMoveRight_x) {
			x = mapMoveRight_x - map->getPosition().x;
		}
		if (new_y < mapMoveDowm_y) {
			y = mapMoveDowm_y - map->getPosition().y;
		}
		if (new_y > mapMoveUp_y) {
			y = mapMoveUp_y - map->getPosition().y;
		}
		auto MapMove = MoveBy::create(1 / 60, Vec2(x, y));	//1/60是一帧所需要的时间（目前一秒60帧）
		map->runAction(MapMove);
	}
}

void GameScene::AddSoldiers(float dt)
{
	this->schedule(schedule_selector(GameScene::AddOneSoldier), 1.0f, 1, 0);
}

void GameScene::AddOneSoldier(float dt)
{
	auto soldier = Unit::create("soldier/0.png", "soldier");
	map->addChild(soldier, 1, num);
	enemyUintMap[num] = soldier;
	num++;
	log("add one soldier");
	soldier->moveTo_directly(Vec2(1000, 1000));
}

void GameScene::AttackSearch(float dt) {
	//std::map<int, Unit*>::iterator iter;
	//Vec2 HerpPosition = hero->getPosition();
	//for (iter = enemyUintMap.begin(); iter != enemyUintMap.end(); iter++) {
	//	if()
	//}
}