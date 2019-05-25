#ifndef GAMESCENE

#include "cocos2d.h"
#include "UnitClass.h"


class GameScene :public Layer {
private:

	TMXTiledMap* map;
	Unit* hero;
	//��Ļ�ߴ���Ƭ�ߴ�
	Size viewSize;
	Size mapSize;
	Size tileSize;
	//�ĸ��ֱ�Ϊmap�ƶ��ı߽磬��ֹmap�Ƴ�����
	float mapMoveLeft_x;
	float mapMoveRight_x;
	float mapMoveUp_y;
	float mapMoveDowm_y;

	GameScene* thisScene;


	struct MapPoint {
		std::vector<int> can_be;
		Vec2 position;
		int has_been = 0;//һ�ɸ��ҵ�ʱ����0������Ҳȫ����0 
	};	
	std::vector<MapPoint> movemark;
	
	std::map<int, Unit*> unitMap;		//�ҷ�Ӣ��С����������map
	std::map<int, Unit*> enemyUintMap;		//�з�Ӣ��С����������map
	


	Vec2 heroSetPosition;		//Ӣ�۳����ص�
	Vec2 soldierSetPosition;	//С�������ص�


public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	bool MapInit();
	bool HeroInit();
	bool PointInit();
	void AllActionsTakenEachF(float dt);			//ÿһ֡�����ã�������λ�ã��ж��Ƿ��ƶ���ͼ���������ܻ�����������ܣ�
	void AddSoldiers(float dt);						//��С��
	void AddOneSoldier(float dt);					//��һ��С��
	void AttackSearch(float dt);
	std::vector<Vec2> MoveFind(const Vec2 startpoint, const Vec2& pos_target);		//Ѱ·

	CREATE_FUNC(GameScene);
};





#define GAMESCENE
#endif // !GAMESCENE
#pragma once
