#ifndef GAMESCENE

#include "cocos2d.h"
#include "UnitClass.h"


class GameScene :public Layer {
private:

	TMXTiledMap* map;
	Unit* hero;
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
	
	std::map<int, Unit*> unitMap;		//我方英雄小兵防御塔的map
	std::map<int, Unit*> enemyUintMap;		//敌方英雄小兵防御塔的map
	


	Vec2 heroSetPosition;		//英雄出生地点
	Vec2 soldierSetPosition;	//小兵出生地点


public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	bool MapInit();
	bool HeroInit();
	bool PointInit();
	void AllActionsTakenEachF(float dt);			//每一帧都调用，监测鼠标位置，判断是否移动地图（后续可能会添加其他功能）
	void AddSoldiers(float dt);						//出小兵
	void AddOneSoldier(float dt);					//出一个小兵
	void AttackSearch(float dt);
	std::vector<Vec2> MoveFind(const Vec2 startpoint, const Vec2& pos_target);		//寻路

	CREATE_FUNC(GameScene);
};





#define GAMESCENE
#endif // !GAMESCENE
#pragma once
