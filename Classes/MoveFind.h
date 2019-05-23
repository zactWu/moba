#ifndef MOVEFIND
#include "cocos2d.h"
USING_NS_CC;


struct MapPoint {
	std::vector<int> can_be;
	Vec2 position;
	int has_been = 0;//一律给我的时候是0，跑完也全部是0 
};

struct route {
	std::vector<Vec2> position;
	float has_move = 0;
	int movemarknum;
};
bool ObjHit(Vec2 p0, TMXTiledMap* map);
bool canto(Vec2 p1, Vec2 p2, TMXTiledMap* map);
bool PointInit(std::vector<MapPoint>& movemarks, TMXTiledMap* map);			//初始化map中障碍物旁边的point，用于寻路
bool operator < (const route c1, const route c2);



#define MOVEFIND
#endif // !MOVEFIND
#pragma once
