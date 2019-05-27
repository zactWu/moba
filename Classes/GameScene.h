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
	
	std::map<int, Unit*> unitMap;
	Vec2 heroSetPosition;
	Vec2 soldierSetPosition;

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void AllActionsTakenEachSecond(float dt);
	bool MapInit();
	bool HeroInit();
	void AllActionsTakenEachF(float dt);			//每一帧都调用，监测鼠标位置，判断是否移动地图（后续可能会添加其他功能）
	std::vector<Vec2> MoveFind(const Vec2 startpoint, const Vec2& pos_target);		//寻路
	bool PointInit();
	CREATE_FUNC(GameScene);
	void SkillHit();
};





#define GAMESCENE
#endif // !GAMESCENE
#pragma once
