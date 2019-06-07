#ifndef GAMESCENE

#include "cocos2d.h"
#include "UnitClass.h"
#include "skillClass.h"
#include "Hero.h"
#include "TowerClass.h"

//GameClient client;// ���÷�����ע�͵�����
class GameScene :public Layer {
private:

	
	Hero* hero;
	Hero* en_hero;
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
	Vec2 heroSetPosition;
	Vec2 soldierSetPosition;

public:
	TMXTiledMap* map;
	static cocos2d::Scene* createScene();
	void TowerInit();
	virtual bool init();
	void SkillHitCheck();
	void UnitDeadAction();
	void TowerAction();
	void AllActionsTakenEachSecond(float dt);
	bool MapInit();
	bool HeroInit();
	void AllActionsTakenEachF(float dt);			//ÿһ֡�����ã�������λ�ã��ж��Ƿ��ƶ���ͼ���������ܻ�����������ܣ�
	void UiShow();
	void AddSoldiers(float dt);
	void AddOneSoldier(float dt);
	void addsoilder();
	std::vector<Vec2> MoveFind(const Vec2 startpoint, const Vec2& pos_target);		//Ѱ·
	bool PointInit();
	std::map<int, Unit*> unit_map;
	std::map<int, Tower*> tower_map;
	std::map<int, Skill*> skill_map;
	int my_hero_id;
	int en_hero_id;//�����ͨ��ȫ�ֿ�
	int unit_num[2] = { 1,1000 };
	int skill_num = 0;
	int tower_num[2] = { 10000,20000 };
	bool SkillHit(Skill* sk, Unit* un);
	// �������͸��ָ����ļ���
	void UsingFireBoll(Unit* hero, Vec2 newPosition, Unit* tar);
	void UsingFireBoll(Unit* sb,Vec2 newPosition);

	CREATE_FUNC(GameScene);
	
	void ListenOutside();

	int ClickFindTag(Vec2 pos);

};





#define GAMESCENE
#endif // !GAMESCENE
#pragma once 
