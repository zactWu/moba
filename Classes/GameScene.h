#ifndef GAMESCENE

#include "cocos2d.h"
#include "UnitClass.h"
#include "skillClass.h"
#include "Hero.h"
#include "TowerClass.h"


//GameClient client;// 不用服务器注释掉这行
class GameScene :public Layer {
private:



	//屏幕尺寸瓦片尺寸
	Size viewSize;
	Size mapSize;
	Size tileSize;
	//四个分别为map移动的边界，防止map移出窗口
	float mapMoveLeft_x;
	float mapMoveRight_x;
	float mapMoveUp_y;
	float mapMoveDowm_y;

	GameScene* thisScene;


	struct MapPoint {
		std::vector<int> can_be;
		Vec2 position;
		int has_been = 0;//一律给我的时候是0，跑完也全部是0 
	};


	std::vector<MapPoint> movemark;
	Vec2 heroSetPosition;
	Vec2 soldierSetPosition;

public:
	TMXTiledMap* map;
	Hero* hero;
	Hero* en_hero;
	static cocos2d::Scene* createScene();
	void TowerInit();
	virtual bool init();
	void SkillHitCheck();
	void UnitDeadAction();
	void UnitDead(Unit* unit);
	void TowerAction();
	void AllActionsTakenEachSecond(float dt);
	bool MapInit();
	bool HeroInit();
	bool ChatInit();
	void AllActionsTakenEachF(float dt);
	void mapMove();//每一帧都调用，监测鼠标位置，判断是否移动地图（后续可能会添加其他功能）
	void UiShow();
	void AddSoldiers(float dt);
	void AddOneSoldier(float dt);
	void SoldierAction();
	void addsoilder();
	std::vector<Vec2> MoveFind(const Vec2 startpoint, const Vec2& pos_target);		//寻路
	bool PointInit();
	std::map<int, Unit*> unit_map;
	std::map<int, Tower*> tower_map;
	std::map<int, Skill*> skill_map;
	int unit_num[2] = { 1,1000 };
	int skill_num = 0;
	int tower_num[2] = { 10000,20000 };
	bool SkillHit(Skill* sk, Unit* un);
	

	CREATE_FUNC(GameScene);

	void ListenOutside();

	int ClickFindTag(Vec2 pos);

	cocos2d::MenuItemImage* equipment;
	void gotoshop(cocos2d::Ref* psender);

	void updatepos(float dt);
};





#define GAMESCENE
#endif // !GAMESCENE
#pragma once 
