#include "GameScene.h"
#include "cocos2d.h"
#include <windows.h>
#include "MoveFind.h"
#include "TowerClass.h"
#include "Hero.h"
#include "control.h"
Vec2 pos[2],tar[2];
#include "client.h"
#include "ui/CocosGUI.h"
#include <mutex>
#include "ShopLayer.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"
#include "GlobalVal.h"
#define SEVER 0
#define MESIDE 0
#define ENEMYSIDE 1
#define MAPZERO 10
#define HEROZERO 15
#define AnimateLimit 15
#define SystemLimit 20
#define Shop 100
#define HEROTAG 1234
#define ENHEROTAG 4321
GameClient client;
extern bool fight;
bool Add = false;
extern std::mutex gameLock;
cocos2d::ui::TextField* textField;

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

	equipment = MenuItemImage::create("shop/shopbutton.png", "shop/shopbutton.png", CC_CALLBACK_1(GameScene::gotoshop, this));//商店
	equipment->setScale(0.2);
	equipment->setPosition(1050,625);
	equipment->setTag(0);
	auto menu = Menu::create(equipment, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,Shop);

	MapInit();			//map初始化
	HeroInit();
	PointInit();
	TowerInit();
	ChatInit();
	client.init(hero, en_hero);
	client.ClientProcess();
	this->schedule(schedule_selector(GameScene::AllActionsTakenEachF));		//设置一个update，每一帧都调用，做各种检测
	this->schedule(schedule_selector(GameScene::AllActionsTakenEachSecond),0.2);



	ListenOutside();
	return true;
}

bool GameScene::MapInit()
{
	map = TMXTiledMap::create("map.tmx");		//创建map
	map->setAnchorPoint(Vec2(0, 0));			//设置锚点为左下角
	map->setPosition(Vec2(0, 0));				//设置位置为窗口左下角
	this->addChild(map,MAPZERO);

	viewSize = Director::getInstance()->getOpenGLView()->getVisibleRect().size;		//初始化窗口大小
	mapSize = map->getMapSize();													//初始化map大小（单位：块）
	tileSize = map->getTileSize();													//初始化瓦片大小（单位：像素）

	//初始化map移动的四个边界，map移动时不可以超出这个边界
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
	Vec2 pos = { mapSize.width * tileSize.width - 100,mapSize.height * tileSize.height - 100 };
	this_computer_side = 1;
	if (this_computer_side == 1) {
		Vec2 temp = pos2;
		pos2 = pos;
		pos = temp;
		auto move = MoveTo::create(0.1, Vec2(viewSize.width - mapSize.width * tileSize.width, viewSize.height - mapSize.height * tileSize.height));
		map->runAction(move);
	}
	std::string herotype;
	switch (hero_id)
	{
	case WARRIOR:
		herotype = "warrior";
		break;
	case ARCHER:
		herotype = "archer";
		break;
	case MAGICIAN:
		herotype = "magician";
		break;
	}
	hero = Hero::create(herotype + "/0.png", herotype);
	hero->_side = this_computer_side;
	this->unit_map[unit_num[0]] = hero;
	hero->_money = this_computer_side;
	hero->reborn_pos = pos2;
	hero->setPosition(pos2);
	hero->setTag(ENHEROTAG);
	hero->_it_tag = ENHEROTAG;
	map->addChild(hero);
	unit_num[0]++;
	hero->Qskill_cd_time = 2000;
	hero->Qskill_last_release_time = 0;

	

	switch (en_hero_id)
	{
	case WARRIOR:
		herotype = "warrior";
		break;
	case ARCHER:
		herotype = "archer";
		break;
	case MAGICIAN:
		herotype = "magician";
		break;
	}
	en_hero = Hero::create(herotype + "/0.png", herotype);

	en_hero->_side =1-this_computer_side;
	en_hero->reborn_pos = pos;
	en_hero->setPosition(en_hero->reborn_pos);
	unit_map[unit_num[1]] = en_hero;
	en_hero->_money = 0;
	en_hero->setTag(HEROTAG);
	en_hero->_it_tag = HEROTAG;
	
	map->addChild(en_hero);
	unit_num[1]++;
	en_hero->Qskill_cd_time = 2000;
	en_hero->Qskill_last_release_time = 0;
	return false;
}

bool GameScene::ChatInit()
{
	//chat
	textField = cocos2d::ui::TextField::create("chat", "Arial", 30);
	textField->setMaxLength(100);
	//下面设置键盘Enter监听，用于聊天
	this->addChild(textField, 10);
	textField->setPosition(Vec2(50, 50));
	textField->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		});
	return true;
}


void GameScene::AllActionsTakenEachSecond(float dt) {
	hero->_money+=1000;// 加钱
	SkillHitCheck();
	UnitDeadAction();
	TowerAction();
	SoldierAction();
//	log("unit_num is %d", unit_map.size());
}
void GameScene::SkillHitCheck() {
	auto skill = skill_map.begin();
	clock_t start = clock();
	static int times = 0;
	times++;
	while (skill != skill_map.end()) {
		int dis = skill->second->getPosition().getDistance(skill->second->_st_pos);
		if (skill->second->targe != NULL && skill->second->targe->_life_current <= 0) {
			skill->second->stopAllActions();
			map->removeChild(skill->second);
			skill = skill_map.erase(skill);
			continue;
		}
		if (skill->second->targe != NULL && skill->second->targe->_side == skill->second->_side) {
			skill->second->stopAllActions();
			map->removeChild(skill->second);
			skill = skill_map.erase(skill);
			continue;
		}
		if (skill->second->targe != NULL && skill->second->targe->_life_current > 0) {
			skill->second->stopAllActions();
			skill->second->move(skill->second->getPosition(), skill->second->targe->getPosition());
		}
		if (dis > skill->second->move_range - 20 && skill->second->targe == NULL) {
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
					// 还有伤害加进去
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
					// 还有伤害加进去
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
	while (unit != unit_map.end()) {// 这一行是用来检查外部引用getdamage的指向性（放出技能的时候就知道能不能打中）技能的
		if (unit->second->_life_current <= 0) {
			UnitDead(unit->second);
			if (unit->second->_life_current < 0) {
				unit = unit_map.erase(unit);
			}
			else {
				++unit;
			}
		}
		else {
			// 先是受伤
			if (unit_map.size() < AnimateLimit && false) {// 动画效果都塞到这里来
//				log("no more animate");
				unit->second->getDamaged();// 重载过的，里面是动画

			}
			// 接下来遍历命令单
			unit->second->update_follow_attack(0.1);
			unit->second->GetOrder();
			++unit;
		}
	}

}

void GameScene::UnitDead(Unit *unit) {
	unit->stopAllActions();
	if (unit->getParent() == NULL) {
		return;
	}
	// 赏金放在这里，升级系统也是
	if (unit->_last_attacker != NULL) {
		if (unit->_last_attacker == hero) {
//			log("right");
			if (unit == en_hero) {
				hero->_kill_times++;
			}
			hero->_money += unit->_kill_award;
			hero->_kill_award +=100;
			if (unit->_last_attacker->_money != hero->_money && (unit->_last_attacker==hero)) {
//				log("pointer wrong!");
			}
			else {
//				log("POINTER right");
			}
		}
		if (unit->_last_attacker == en_hero) {
			if (unit == hero) {
				en_hero->_kill_times++;
			}
		}
//		cocos2d::log("you(%d) should get %d money and now is %d", unit->_last_attacker->_it_tag,unit->_kill_award, hero->_money);
	}
	//死亡动画是下面这段
	if (unit_map.size() <= AnimateLimit) {
		Sprite* money;
		if (unit->_last_attacker == hero) {
			money = Sprite::create("money.jpg");
		}
		else {
			money = Sprite::create("angle.jpg");
		}
		money->setPosition(unit->getPosition());
		money->setScale(0.1);
		map->addChild(money);
		auto fed = FadeOut::create(1.0f);
		money->runAction(fed);
		// 死亡动画结束
	}
	
	if (unit == hero) {// 这里是复活的
		hero->_dead_times++;
		hero->skill_statement = 0;
		hero->_life_current = hero->_life_max;
		hero->setPosition(hero->reborn_pos);
		hero->getDamaged(NULL, 1);
		hero->_money -= 50;
		return;
	}
	if (unit == en_hero) {
		en_hero->_dead_times++;
		en_hero->skill_statement = 0;
		en_hero->_life_current = en_hero->_life_max;
		en_hero->setPosition(en_hero->reborn_pos);
		en_hero->getDamaged(NULL, 1);
		en_hero->_money -= 50;
		return;
	}
	map->removeChild(unit);
}
void GameScene::TowerAction() {
	auto tower = tower_map.begin();
	while (tower != tower_map.end()) {// 这一行是用来检查外部引用getdamage的指向性（放出技能的时候就知道能不能打中）技能的
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
			// 胜利条件做在这里
		}
		else {
			float pass_time = clock() - tower->second->_last_release_time;
			//log("pass time is %f", pass_time);
			if (pass_time > tower->second->_cd_time) {
				float best_dis = tower->second->hit_range;
				Unit* firetar = NULL;
				auto unit = unit_map.begin();
				//log("has check");
				while (unit != unit_map.end()) {
					if (unit->second->getPosition().getDistance(tower->second->getPosition())
						< best_dis &&
						unit->second->_side != tower->second->_side - 2 &&
						unit->second->_life_current > 0) {

						best_dis = unit->second->getPosition().getDistance(tower->second->getPosition());
						firetar = unit->second;

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

void GameScene::AllActionsTakenEachF(float dt)
{

	if (false == Add && true == fight) {		//未出过兵以及开始战斗
		this->schedule(schedule_selector(GameScene::AddSoldiers), 35.0f);		//十五秒出一波兵
		log("Add");
		Add = true;			//出过兵了
	}
	UiShow();
	mapMove();
	//50是我设定的区域，鼠标位于该区域内则会导致map移动，50可更改
	
}

void GameScene::mapMove() {
	int RectWidth = viewSize.width;		//窗口宽
	int RectHeight = viewSize.height;	//窗口高
	//以下几行获取鼠标位于窗口的坐标
	POINT p;
	GetCursorPos(&p);
	HWND hwnd = GetActiveWindow();
	ScreenToClient(hwnd, &p);
	p.y = RectHeight - p.y;
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
void GameScene::UiShow() {
	//显示聊天信息
	if (true == client.UpdateChatMessage) {
		if (this->getChildByName("ChatMessage") != nullptr) {
			this->removeChildByName("ChatMessage");
		}
		auto ChatMessage = Label::createWithSystemFont(client.ChattingInfirmationFromTheOther, "Arial", 25);
		if (ChatMessage != nullptr)
		{
			ChatMessage->setPosition(Vec2(50, 100));
			this->addChild(ChatMessage, 10);
			ChatMessage->setName("ChatMessage");
		}
		client.UpdateChatMessage = false;
	}
	//等级
	if (this->getChildByName("levelLabel") != nullptr) {
		this->removeChildByName("levelLabel");
	}
	char l[20];
	int level = hero->_kill_award / 300;
	sprintf_s(l, "level: %d", level);
	auto levelLabel = Label::createWithSystemFont(l, "Arial", 30);
	if (levelLabel != nullptr)
	{
		levelLabel->setPosition(Vec2(100, 200));
		this->addChild(levelLabel, 10);
		levelLabel->setName("levelLabel");
	}
	//战绩
	if (this->getChildByName("MyScore") != nullptr) {
		this->removeChildByName("MyScore");
	}
	char score[20] = { 123 };
	sprintf_s(score, "%d : %d", hero->_kill_times,hero->_dead_times);
	auto MyScore = Label::createWithSystemFont(score, "Arial", 30);
	if (MyScore != nullptr)
	{
		MyScore->setPosition(Vec2(viewSize.width / 2, viewSize.height - 100));
		this->addChild(MyScore, 10);
		MyScore->setName("MyScore");
	}
	//money
	if (this->getChildByName("MoneyLabel") != nullptr) {
		this->removeChildByName("MoneyLabel");
	}
	char m[20];
	sprintf_s(m, "money: %d", hero->_money);
	auto MoneyLabel = Label::createWithSystemFont(m, "Arial", 20);
	if (MoneyLabel != nullptr)
	{
		MoneyLabel->setPosition(Vec2(viewSize.width - 200, 100));
		this->addChild(MoneyLabel, 10);
		MoneyLabel->setName("MoneyLabel");
	}
	//cd
	if (this->getChildByName("CdLabel") != nullptr) {
		this->removeChildByName("CdLabel");
	}
	if (this->getChildByName("Q") != nullptr) {
		this->removeChildByName("Q");
	}
	if (this->getChildByName("W") != nullptr) {
		this->removeChildByName("W");
	}
	if (this->getChildByName("E") != nullptr) {
		this->removeChildByName("E");
	}
	float Q_Cd = this->hero->Qskill_cd_time + this->hero->Qskill_last_release_time - clock();
	float W_Cd = this->hero->Wskill_cd_time + this->hero->Wskill_last_release_time - clock();
	float E_Cd = this->hero->Eskill_cd_time + this->hero->Eskill_last_release_time - clock();
	Q_Cd /= 100;
	W_Cd /= 100;
	E_Cd /= 100;
	//log("%f %f %f", Q_Cd, W_Cd, E_Cd);
	if (Q_Cd <= 0 && W_Cd <= 0 && E_Cd <= 0) {
		char cd[100] = { 0 };
		sprintf_s(cd, "%d ready:", hero->skill_statement);
		auto CdLabel = Label::createWithSystemFont(cd, "Arial", 20);
		if (CdLabel != nullptr) {
			this->addChild(CdLabel, 10);
			CdLabel->setPosition(Vec2(viewSize.width - 200, 75));
			CdLabel->setName("CdLabel");
		}
	}
	else {
		if (Q_Cd > 0) {
			char Q[100];
			sprintf_s(Q, "Q skill cd: %f", Q_Cd);
			auto QCdLabel = Label::createWithSystemFont(Q, "Arial", 20);
			if (QCdLabel != nullptr) {
				this->addChild(QCdLabel, 10);
				QCdLabel->setPosition(Vec2(viewSize.width - 200, 75));
				QCdLabel->setName("Q");
			}
		}
		if (W_Cd > 0) {
			char W[100];
			sprintf_s(W, "W skill cd: %f", W_Cd);
			auto WCdLabel = Label::createWithSystemFont(W, "Arial", 20);
			if (WCdLabel != nullptr) {
				this->addChild(WCdLabel, 10);
				WCdLabel->setPosition(Vec2(viewSize.width - 200, 50));
				WCdLabel->setName("W");
			}
		}
		if (E_Cd > 0) {
			char E[100];
			sprintf_s(E, "E skill cd: %f", E_Cd);
			auto ECdLabel = Label::createWithSystemFont(E, "Arial", 20);
			if (ECdLabel != nullptr) {
				this->addChild(ECdLabel, 10);
				ECdLabel->setPosition(Vec2(viewSize.width - 200, 25));
				ECdLabel->setName("E");
			}
		}
	}
}
void GameScene::AddSoldiers(float dt)
{
	this->schedule(schedule_selector(GameScene::AddOneSoldier), 1.0f, 2, 0);
}

void GameScene::AddOneSoldier(float dt)
{
	if (unit_map.size() >= SystemLimit) {
		log("no more soider");
		return;
	}
	
	pos[0] = { 50,50 };
	pos[1] = { 2500,2500 };
	log("come out!");// 这个可以修改为出兵的出生点和到达点
	static int kind_by_time = 0;
	std::string my_name;
	std::string en_name;
	int acr = 0;
	switch (kind_by_time%3)
	{
	case 0:
		my_name = "soldier_blue";
		en_name = "soldier_yellow";
		acr = 80;
		break;
	case 1:
		my_name = "long_green";
		en_name = "long_brown";
		acr = 200;
		break;
	case 2:
		my_name = "helicopter1";
		en_name = "helicopter2";
		acr = 280;
		break;

	}
	auto soldier = Unit::create(my_name + "/0.png", my_name);//暂时只有我方
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
	soldier->_kill_award = 100;
	soldier->_attackRange = acr;
	// 接下来是敌方

	auto soldier2 = Unit::create(en_name + "/0.png", en_name);
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
	soldier2->_kill_award = 100;
	soldier2->_attackRange = acr;
	kind_by_time++;
}

void GameScene::SoldierAction() {
	auto unit = unit_map.begin();
	while (unit != unit_map.end()) {
		if (unit->second->_life_current <= 0 || unit->second->_kind!=1) {
			++unit;// 你不是士兵，你到底是谁？
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


void GameScene::gotoshop(Ref* psender) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto winsize = Director::getInstance()->getWinSize();
	if (equipment->getTag() == 0)
	{
		Sprite* window = Sprite::create("shop/window.png");
		auto contentsize = window->getContentSize();
		window->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		window->setScale(winsize.width * 0.8 / contentsize.width, winsize.height * 0.5 / contentsize.height);
		this->addChild(window, Shop);
		ShopLayer* scrollView = ShopLayer::createLayer(hero);
		window->setName("rm1");
		scrollView->setName("rm2");
		this->addChild(scrollView,101);
		equipment->setTag(1);
	}
	else
	{
		this->removeChildByName("rm1");
		this->removeChildByName("rm2");
		equipment->setTag(0);
	}

}


