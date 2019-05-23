#ifndef MOVEFIND
#include "cocos2d.h"
USING_NS_CC;

struct MapPoint {
	std::vector<int> can_be;
	Vec2 position;
	int has_been = 0;//һ�ɸ��ҵ�ʱ����0������Ҳȫ����0 
};


struct route {
	std::vector<Vec2> position;
	float has_move = 0;
	int movemarknum;
};
bool ObjHit(Vec2 p0, TMXTiledMap* map);
bool canto(Vec2 p1, Vec2 p2, TMXTiledMap* map);
bool MapInit(std::vector<MapPoint>& movemarks, TMXTiledMap* map);
bool operator < (const route c1, const route c2);
std::vector<Vec2> MoveFind(const Vec2 startpoint, const Vec2& pos_target, std::vector<MapPoint> movemark, TMXTiledMap* map);


#define MOVEFIND
#endif // !MOVEFIND
#pragma once
