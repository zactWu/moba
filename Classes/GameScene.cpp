#include "GameScene.h"
#include "cocos2d.h"
#include <windows.h>
#include "MoveFind.h"
#include "TowerClass.h"
#include "Hero.h"
//#include "client.h"
#define SEVER 0
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
	TowerInit();
	Vec2 pos = { 400,400 };
	auto spa = Unit::create("soldier/0.png", "soldier");
	unit_map[unit_num[1]] = spa;
	spa->setTag(unit_num[1]);
	unit_num[1]--;
	spa->_side = 1;
	spa->setPosition(pos);
	map->addChild(spa);
	
	spa->_it_tag = unit_num[1];
	if (SEVER) {
		
		//client.ClientProcess();
	}
	this->schedule(schedule_selector(GameScene::AllActionsTakenEachF));		//����һ��update��ÿһ֡�����ã������ּ��
	this-> schedule(schedule_selector(GameScene::AllActionsTakenEachSecond),0.15);
	ListenOutside();

	
	return true;
}
void GameScene::SkillHitCheck() {
	auto skill = skill_map.begin();
	clock_t start = clock();
	static int times = 0;
	times++;
	while (skill != skill_map.end()) {
		int dis = skill->second->getPosition().getDistance(skill->second->_st_pos);
		if (skill->second->targe != NULL && skill->second->targe->_life_current>0) {
			skill->second->stopAllActions();
			skill->second->move(skill->second->getPosition(), skill->second->targe->getPosition());
		}
		if (skill->second->targe != NULL && skill->second->targe->_life_current <= 0) {
			skill->second->stopAllActions();
			map->removeChild(skill->second);
			skill = skill_map.erase(skill);
			continue;
		}
		if (dis > skill->second->move_range - 20 && skill->second->targe==NULL) {
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
					unit->second->getDamaged(skill->second->_skiller, skill->second->_damage);
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
			if (flag)++skill;
		}
	}
	
}
void GameScene::UnitDeadAction() {
	auto unit = unit_map.begin();
	while (unit != unit_map.end()) {// ��һ������������ⲿ����getdamage��ָ���ԣ��ų����ܵ�ʱ���֪���ܲ��ܴ��У����ܵ�
		if (unit->second->_life_current <= 0) {
			unit->second->stopAllActions();
			if (unit->second == hero) {
				hero->_life_current = 100;
				hero->setPosition(hero->reborn_pos);
			}
			if (unit->second->_last_attacker != NULL) {
				unit->second->_last_attacker->_money += unit->second->_kill_award;// �ͽ��������
			}
			auto money = Sprite::create("money.jpg");
			money->setPosition(unit->second->getPosition());
			money->setScale(0.2);
			map->addChild(money);
			auto fed = FadeOut::create(1.0f);
			money->runAction(fed);
			if (unit->second == hero) {// �����Ǹ����
				hero->_life_current = 100;
				hero->setPosition(hero->reborn_pos);
				hero->getDamaged(NULL, 1);
				hero->_money -= 50;
				++unit;
				continue;
			}
			map->removeChild(unit->second);
			unit = unit_map.erase(unit);
		}
		else {
			// �������������
			for (auto i = unit->second->order_list.begin(); i != unit->second->order_list.end();) {
				if (i->kind == 1) {
					unit->second->moveTo_directly(MoveFind(unit->second->getPosition(), i->pos));
					log("unit move");
				}
				if (i->kind == 2) {
					// �ٶ������Ӣ��
					int tag = unit->second->_it_tag;
					log("TAG IS %d", unit->second->_it_tag);
					if (hero == unit->second) {
						if (hero->Qskill_last_release_time + hero->Qskill_cd_time < clock()){
                            hero->UsingFireBall(i->pos);// ����ĳɺ���Ӣ�۵ļ��ܾͺ�
							hero->Qskill_last_release_time = clock();
						}
					}
				}
				i = unit->second->order_list.erase(i);
			}
			++unit;
		}
	}

}
void GameScene::TowerAction() {
	auto tower = tower_map.begin();
	while (tower != tower_map.end()) {// ��һ������������ⲿ����getdamage��ָ���ԣ��ų����ܵ�ʱ���֪���ܲ��ܴ��У����ܵ�
		if (tower->second->_life_current <= 0) {
			auto money = Sprite::create("towercrash.jpg");
			money->setPosition(tower->second->getPosition());
			money->setScale(0.03);
			if (tower->second->_last_attacker != NULL) {
				tower->second->_last_attacker->_money += tower->second->_kill_award;
			}
			map->addChild(money);
			map->removeChild(tower->second);
			
			
			tower = tower_map.erase(tower);
		}
		else {
			float pass_time = clock() - tower->second->_last_release_time;
			//log("pass time is %f", pass_time);
			if (pass_time > tower->second->_cd_time) {

				auto unit = unit_map.begin();
				//log("has check");
				while (unit != unit_map.end()) {
					if (unit->second->getPosition().getDistance(tower->second->getPosition())
						< tower->second->hit_range &&
						unit->second->_side!=tower->second->_side-2) {
						tower->second->fire(unit->second);
						tower->second->_last_release_time = clock();
						log("in range!!");
						//tower->second->getDamaged(tower->second, 200);
						break;
					}
					++unit;
				}
			}
			++tower;
		}
	}
}
void GameScene::AllActionsTakenEachSecond(float dt) {
	hero->_money++;// ��Ǯ
	SkillHitCheck();
	UnitDeadAction();
	TowerAction();
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
void GameScene::TowerInit() {
	auto towerA = Tower::create("tower.jpg", 3);
	Vec2 pos = { 600,600 };
	towerA->setPosition(pos);
	towerA->setScale(0.1);
	map->addChild(towerA);
	//log("towerA ready");
	tower_map[tower_num[0]] = towerA;
	tower_num[0]++;
	return;
}
bool GameScene::HeroInit()
{
	hero = Hero::create("soldier/0.png", "soldier");
	hero->_side = MESIDE;
	this->unit_map[unit_num[0]] = hero;
	hero->_money = 0;
	
	hero->setTag(unit_num[0]);
	hero->_it_tag = unit_num[0];
	map->addChild(hero,HEROZERO);
	unit_num[0]++;
	hero->Qskill_cd_time = 2000;
	hero->Qskill_last_release_time = 0;
	//client.init(hero);
	return false;
}
void GameScene::AllActionsTakenEachF(float dt)
{

	if (this->getChildByName("MoneyLabel") != nullptr) {
		this->removeChildByName("MoneyLabel");
	}
	float  money = this->hero->Qskill_cd_time+ this->hero->Qskill_last_release_time-clock();
	money /= 100;
	char m[1000];
	if (money > 0) {
		sprintf_s(m, "money: %f", money);
	}
	else
	{
		sprintf_s(m, "money: %f", 0.0);
	}
	auto MoneyLabel = Label::createWithSystemFont(m, "Arial", 25);
	if (MoneyLabel != nullptr)
	{
		//log("money %d", money);
		// position the label on the center of the screen
		MoneyLabel->setPosition(Vec2(viewSize.width - 200, 100));

		// add the label as a child to this layer
		this->addChild(MoneyLabel, 10);
		MoneyLabel->setName("MoneyLabel");
	}

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



