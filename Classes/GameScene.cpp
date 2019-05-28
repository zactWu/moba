#include "GameScene.h"
#include "cocos2d.h"
#include <windows.h>
#include "MoveFind.h"
#define MESIDE 0
#define ENEMYSIDE 1
#define MAPZERO 10
#define HEROZERO 15
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
	MapInit();			//map��ʼ��
	HeroInit();
	PointInit();
	Vec2 pos = { 400,400 };
	auto spa = Unit::create("soldier/0.png", "soldier");
	unit_map[unit_num] = spa;
	unit_num++;
	spa->_side = ENEMYSIDE;
	spa->setPosition(pos);
	map->addChild(spa);
	this->schedule(schedule_selector(GameScene::AllActionsTakenEachF));		//����һ��update��ÿһ֡�����ã������ּ��
	
	this-> schedule(schedule_selector(GameScene::AllActionsTakenEachSecond),0.1);
																			//������
	/*
	auto bloodBg = Sprite::create(p_bloodline);	
	bloodBg->setPosition(Point(npc->getContentSize().width / 2, npc->getContentSize().height - 10));
	npc->addChild(bloodBg, 1);
	auto bloodBlue = Sprite::create(p_bloodlinehong);
	*/
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch * Touch, Event * Event) {
		auto touchPosition = Touch->getLocation();
		auto mapPosition = map->getPosition();
		auto nowPosition = hero->getPosition();
		auto newPosition = touchPosition - mapPosition;
		std::vector<Vec2> route = MoveFind(hero->getPosition(), newPosition);
		hero->moveTo_directly(route);
		UsingFireBoll(hero, newPosition);
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
void GameScene::AllActionsTakenEachSecond(float dt) {
	hero->_money++;// ��Ǯ

	auto skill = skill_map.begin();
	clock_t start = clock();
	static int times = 0;
	times++;
	while (skill != skill_map.end()) {
		int dis = skill->second->getPosition().getDistance(skill->second->_st_pos);
		if (dis > skill->second->move_range - 50) {
			map->removeChild(skill->second);
			skill = skill_map.erase(skill);
			continue;
		}
		else {
			bool flag = 1;
			auto unit = unit_map.begin();
			while (unit != unit_map.end()) {
				if (this->SkillHit(skill->second, unit->second)) {
					log("HIT!!");
					// �����˺��ӽ�ȥ
					map->removeChild(skill->second);
					skill = skill_map.erase(skill);
					flag = 0;
					break;
				}
				else
				{
					++unit;
				}
			}
			if(flag)++skill;
		}
		
	}
}
bool GameScene::MapInit()
{
	map = TMXTiledMap::create("map.tmx");		//����map
	map->setAnchorPoint(Vec2(0, 0));			//����ê��Ϊ���½�
	map->setPosition(Vec2(0, 0));				//����λ��Ϊ�������½�
	this->addChild(map,MAPZERO);

	viewSize = Director::getInstance()->getOpenGLView()->getVisibleRect().size;		//��ʼ�����ڴ�С
	mapSize = map->getMapSize();													//��ʼ��map��С����λ���飩
	tileSize = map->getTileSize();													//��ʼ����Ƭ��С����λ�����أ�

	//��ʼ��map�ƶ����ĸ��߽磬map�ƶ�ʱ�����Գ�������߽�
	mapMoveRight_x = 0;
	mapMoveLeft_x = viewSize.width - map->getMapSize().width * map->getTileSize().width;
	mapMoveUp_y = 0;
	mapMoveDowm_y = viewSize.height - map->getMapSize().height * map->getTileSize().height;

	
	return false;
}

bool GameScene::HeroInit()
{
	hero = Unit::create("soldier/0.png", "soldier");
	hero->_side = MESIDE;
	this->unit_map[unit_num] = hero;
	unit_num++;

	map->addChild(hero,HEROZERO);
	return false;
}



void GameScene::AllActionsTakenEachF(float dt)
{

	int RectWidth = viewSize.width;		//���ڿ�
	int RectHeight = viewSize.height;	//���ڸ�
	//���¼��л�ȡ���λ�ڴ��ڵ�����
	POINT p;
	GetCursorPos(&p);
	HWND hwnd = GetActiveWindow();
	ScreenToClient(hwnd, &p);
	p.y = RectHeight - p.y;

	//50�����趨���������λ�ڸ���������ᵼ��map�ƶ���50�ɸ���
	if (RectWidth - p.x < 50 || RectHeight - p.y < 50 || p.x < 50 || p.y < 50) {
		//�������е� 10 ���Զ������õģ���ÿһ�趨��ʱ�䣨Ŀǰ�趨һ֡����Ҫ��ʱ�䣩�ƶ�10�����أ��ɸ��ģ����⣬sqrt���ڿ����Ų�����������룬�о�ʽ���е�low.....Ҫ���а취д�øߴ���һ�������
		auto x = 10 * (RectWidth / 2 - p.x) / sqrt((RectWidth / 2 - p.x) * (RectWidth / 2 - p.x) + (RectHeight / 2 - p.y) * (RectHeight / 2 - p.y));
		auto y = 10 * (RectHeight / 2 - p.y) / sqrt((RectHeight / 2 - p.y) * (RectHeight / 2 - p.y) + (RectWidth / 2 - p.x) * (RectWidth / 2 - p.x));
		//�ȼ���һ���ƶ����new_x,new_y����
		auto new_x = map->getPosition().x + x;
		auto new_y = map->getPosition().y + y;
		//�����ƶ����new_x,new_y����ᵼ��map�������ڣ������x,y����
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
		auto MapMove = MoveBy::create(1 / 60, Vec2(x, y));	//1/60��һ֡����Ҫ��ʱ�䣨Ŀǰһ��60֡��
		map->runAction(MapMove);
		// 
		
		
		//
	}
}



