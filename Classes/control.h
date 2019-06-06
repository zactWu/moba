#ifndef CONTROL
#define CONTROL

#include "cocos2d.h"
#include "UnitClass.h"
#include "GameScene.h"
#define MOVEOERDER 1
#define SKILLQ 2
#define SKILLW 3
#define SKILLE 4
#define REBRON 5
#define DEAD 6
#define ATTACK 7
USING_NS_CC;

class control {
public:
	Vec2 pos;
	Unit* tar_point = NULL;
	int tar_tag = -1;
	int kind;
	bool checkself();
	void send_to_sever();
	void ListenOutside();
};

#endif // !UNITCLASS