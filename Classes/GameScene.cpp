#include "GameScene.h"
#include "cocos2d.h"
#include <windows.h>
#include "MoveFind.h"
#include "TowerClass.h"
#include "Hero.h"
Vec2 pos[2],tar[2];
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
	
	
	
	this->schedule(schedule_selector(GameScene::AllActionsTakenEachF));		//����һ��update��ÿһ֡�����ã������ּ��
	this-> schedule(schedule_selector(GameScene::AllActionsTakenEachSecond),0.2);
	this->schedule(schedule_selector(GameScene::AddSoldiers), 15.0f);		//ʮ�����һ����

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
		if (skill->second->targe != NULL && skill->second->targe->_life_current<=0) {
			skill->second->stopAllActions();
			map->removeChild(skill->second);
			skill = skill_map.erase(skill);
			continue;
		}
		if (skill->second->targe != NULL && skill->second->targe->_side==skill->second->_side) {
			skill->second->stopAllActions();
			map->removeChild(skill->second);
			skill = skill_map.erase(skill);
			continue;
		}
		if (skill->second->targe != NULL && skill->second->targe->_life_current>0) {
			skill->second->stopAllActions();
			skill->second->move(skill->second->getPosition(), skill->second->targe->getPosition());
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
			if (!flag) {
				continue;
			}
			auto tower = tower_map.begin();
			while (tower != tower_map.end()) {
				if (skill->second->targe == tower->second &&
					skill->second->getPosition().getDistance(tower->second->getPosition()) < skill->second->hit_range) {
					log("HIT tower!!");
					// �����˺��ӽ�ȥ
					tower->second->getDamaged(skill->second->_skiller, skill->second->_damage);
					map->removeChild(skill->second);
					skill = skill_map.erase(skill);
					flag = 0;
					break;
				}
				else
				{
					++tower;
				}
			}
			

			if (flag) {
				++skill;
			}
			
		}
	}
	
}
void GameScene::UnitDeadAction() {
	auto unit = unit_map.begin();
	while (unit != unit_map.end()) {// ��һ������������ⲿ����getdamage��ָ���ԣ��ų����ܵ�ʱ���֪���ܲ��ܴ��У����ܵ�
		if (unit->second->_life_current <= 0) {
			unit->second->stopAllActions();
			if (unit->second->_last_attacker != NULL) {
				unit->second->_last_attacker->_money += unit->second->_kill_award;// �ͽ�����������ϵͳҲ��
			}
			auto money = Sprite::create("angle.jpg");
			money->setPosition(unit->second->getPosition());
			money->setScale(0.1);
			map->addChild(money);
			auto fed = FadeOut::create(1.0f);
			money->runAction(fed);
			if (unit->second == hero) {// �����Ǹ����
				hero->skill_statement = 0;
				hero->_life_current = hero->_life_max;
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
			// ��������
			if (unit->second->wound) {
				
            auto cf_intoRed = CallFunc::create([=]() {
            if(unit->second!=NULL)
				unit->second->setColor(cocos2d::Color3B::RED);
            });
            auto cf_back = CallFunc::create([=]() {
			if(unit->second !=NULL)
				unit->second->setColor(cocos2d::Color3B(255, 255, 255));
			});
			auto switchColor = Sequence::create(cf_intoRed, DelayTime::create(0.15), cf_back, nullptr);
			unit->second->getParent()->runAction(switchColor);
			unit->second->wound = 0;
			}
			// �������������
			unit->second->update_follow_attack(0.1);
			for (auto i = unit->second->order_list.begin(); i != unit->second->order_list.end();) {
			
				if (i->kind ==1) {
					unit->second->_tag_attackTarget = i->tag;
					if (i->tag == -1) {// ���Ч�������Լ�ȡ��
						hero->_onAttack = 0;
					}
					unit->second->moveTo_directly(MoveFind(unit->second->getPosition(), i->pos));
					//log("unit move");
				}
				if (i->kind != 1) {
					// �ٶ������Ӣ��
					unit->second->_onAttack = 0;
					if (hero == unit->second) {
						hero->useSkill(2, i->kind, i->pos, i->tag);
					}
					else if (en_hero == unit->second) {
						en_hero->useSkill(3, i->kind, i->pos, i->tag);
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
			tower->second->stopAllActions();
			auto money = Sprite::create("towercrash.jpg");
			money->setPosition(tower->second->getPosition());
			money->setScale(0.03);
			if (tower->second->_last_attacker != NULL) {
				tower->second->_last_attacker->_money += tower->second->_kill_award;
			}
			map->addChild(money);
			map->removeChild(tower->second);
			tower = tower_map.erase(tower);
			// ʤ��������������
		}
		else {
			float pass_time = clock() - tower->second->_last_release_time;
			//log("pass time is %f", pass_time);
			if (pass_time > tower->second->_cd_time) {
				float best_dis = tower->second->hit_range;
				Unit* firetar=NULL;
				auto unit = unit_map.begin();
				//log("has check");
				while (unit != unit_map.end()) {
					if (unit->second->getPosition().getDistance(tower->second->getPosition())
						<  best_dis&&
						unit->second->_side!=tower->second->_side-2 &&
						unit->second->_life_current>0) {

						best_dis = unit->second->getPosition().getDistance(tower->second->getPosition());
						firetar = unit->second;
						
					
						
						log("ready tar is %d best dis is %f!!", unit->second->_it_tag, best_dis);
						log("ready tar is %d best dis is %f!!", unit->second->_it_tag, best_dis);
						log("ready tar is %d best dis is %f!!", unit->second->_it_tag, best_dis);
						log("ready tar is %d best dis is %f!!", unit->second->_it_tag, best_dis);
						log("ready tar is %d best dis is %f!!", unit->second->_it_tag, best_dis);
						//tower->second->getDamaged(tower->second, 200);
						
					}
					++unit;
				}
				if (firetar != NULL) {
					tower->second->_last_release_time = clock();
					tower->second->fire(firetar);
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
	SoldierAction();
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
	Vec2 pos = {900,900 };
	towerA->setPosition(pos);
	towerA->setScale(0.1);
	towerA->_side = 2;
	map->addChild(towerA);
	//log("towerA ready");
	tower_map[tower_num[0]] = towerA;
	towerA->setTag(tower_num[0]);
	tower_num[0]++;

    pos = { 1600,1600 };
	auto towerB = Tower::create("tower.jpg", 3);
	
	towerB->setPosition(pos);
	towerB->setScale(0.1);
	towerB->_side = 3;
	map->addChild(towerB);
	//log("towerA ready");
	tower_map[tower_num[1]] = towerB;
	towerB->setTag(tower_num[1]);
	tower_num[1]++;
	return;
}
bool GameScene::HeroInit()
{
	Vec2 pos2 = { 100,100 };
	hero = Hero::create("soldier/0.png", "soldier");
	hero->_side = MESIDE;
	this->unit_map[unit_num[0]] = hero;
	hero->_money = 0;
	hero->reborn_pos = pos2;
	hero->setPosition(pos2);
	hero->setTag(unit_num[0]);
	hero->_it_tag = unit_num[0];
	map->addChild(hero);
	unit_num[0]++;
	hero->Qskill_cd_time = 2000;
	hero->Qskill_last_release_time = 0;

	Vec2 pos = { 400,400 };
	en_hero = Hero::create("soldier/0.png", "soldier");
	en_hero->_side =1;
	en_hero->setPosition(pos);
	unit_map[unit_num[1]] = en_hero;
	en_hero->_money = 0;
	en_hero->setTag(unit_num[1]);
	en_hero->_it_tag = unit_num[1];
	en_hero->reborn_pos = pos;
	map->addChild(en_hero);
	unit_num[1]++;
	en_hero->Qskill_cd_time = 2000;
	en_hero->Qskill_last_release_time = 0;
	//client.init(hero);
	return false;
}
void GameScene::AllActionsTakenEachF(float dt)
{
	UiShow();
	

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

void GameScene::UiShow() {
	if (this->getChildByName("MoneyLabel") != nullptr) {
		this->removeChildByName("MoneyLabel");
	}
	float  money = this->hero->Qskill_cd_time + this->hero->Qskill_last_release_time - clock();
	money /= 100;
	char m[1000];
	if (money > 0) {
		sprintf_s(m, "%d skill cd: %f", hero->skill_statement, money);
	}
	else
	{
		sprintf_s(m, "%d ready:", hero->skill_statement);
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
}

void GameScene::AddSoldiers(float dt)
{
	this->schedule(schedule_selector(GameScene::AddOneSoldier), 1.0f, 1, 0);
}

void GameScene::AddOneSoldier(float dt)
{
	pos[0] = { 50,50 };
	pos[1] = { 2500,2500 };
	log("come out!");
	static int kind_by_time = 0;
	auto soldier = Unit::create("soldier/0.png", "soldier");//��ʱֻ���ҷ�
	soldier->_side = 0;
	unit_map[unit_num[soldier->_side]] = soldier;
	soldier->setTag(unit_num[soldier->_side]);
	soldier->_it_tag = unit_num[soldier->_side];
	unit_num[soldier->_side]++;
	soldier->_money = 0;
	soldier->setPosition(pos[soldier->_side]);
	soldier->_kind = 1;
	map->addChild(soldier);
	soldier->moveTo_directly(pos[1- soldier->_side]);
	// �������ǵз�

	auto soldier2 = Unit::create("soldier/0.png", "soldier");//��ʱֻ���ҷ�
	soldier2->_side = 1;
	unit_map[unit_num[soldier2->_side]] = soldier2;
	soldier2->setTag(unit_num[soldier2->_side]);
	soldier2->_it_tag = unit_num[soldier2->_side];
	unit_num[soldier2->_side]++;
	soldier2->_money = 0;
	soldier2->setPosition(pos[soldier2->_side]);
	soldier2->_kind = 1;
	map->addChild(soldier2);
	soldier2->moveTo_directly(pos[1-soldier2->_side]);

	
}

void GameScene::SoldierAction() {
	auto unit = unit_map.begin();
	while (unit != unit_map.end()) {// ��һ������������ⲿ����getdamage��ָ���ԣ��ų����ܵ�ʱ���֪���ܲ��ܴ��У����ܵ�
		if (unit->second->_life_current <= 0 || unit->second->_kind!=1) {
			++unit;
		}
		else {
			//log("find tag of s");
			auto enemy = unit_map.begin();
			float dis = unit->second->_attackRange;
			int tag = -1;
			while (enemy != unit_map.end()) {
				if (enemy->second->_side == unit->second->_side || enemy->second->_life_current < 0) {
					++enemy;
				}
				else {
					if (unit->second->getPosition().getDistance(enemy->second->getPosition()) < dis) {

						tag = enemy->second->getTag();
						//log("find %d",tag);
					}
					++enemy;
				}
			}
			auto tower = tower_map.begin();
			while (tower != tower_map.end()) {
				if (tower->second->_side-2 == unit->second->_side || tower->second->_life_current < 0) {
					++tower;
				}
				else {
					if (unit->second->getPosition().getDistance(tower->second->getPosition()) < dis) {

						tag = tower->second->getTag();
						//log("find %d",tag);
					}
					++tower;
				}
			}
			unit->second->_tag_attackTarget = tag;
			//log("this soilder tag is %d",tag);
			if (tag == -1) {
				unit->second->moveTo_directly(pos[1- unit->second->_side]);
			}
			
			++unit;
		}
	}
}