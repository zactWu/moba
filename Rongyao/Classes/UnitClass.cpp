#include "UnitClass.h"
#include "cocos2d.h"
#include "debughelper.h"
#include "helper.h"
USING_NS_CC;

//Unit的create函数，调用：myUnit = Unit::create(string,int,int,int,int)
Unit* Unit::create(const std::string &filename, const std::string &unitType,
	int maxLife, int attack, int defense, int speed, float rotate_speed){

	auto unit = new (std::nothrow) Unit;
	if (unit&&unit->initWithFile(filename)) {

		//初始化unit各项属性
		unit->_life_max = maxLife;
		unit->_life_current = maxLife;
		unit->_attack = attack;
		unit->_defense = defense;
		unit->_speed = speed;
		unit->_unitType = unitType;

		unit->autorelease();
		return unit;
	}
	
	CC_SAFE_DELETE(unit);
	return nullptr;
}

//
bool Unit::moveTo_directly(const Vec2 &pos_target) {


	float distance = pos_target.distance(this->getPosition());
	float time_consumed = distance / _speed;
	auto move = MoveTo::create(time_consumed, pos_target);
	this->runAction(move);
	pos_moveTarget = pos_target;

	return true;
}








inline void Unit::animate_move_forever(int dir) {
	 Vector<SpriteFrame*> animFrames;
	 animFrames.reserve(2);
	 animFrames.pushBack(SpriteFrame::create(
		 this->_unitType + "/" + std::to_string(dir) + "_" + std::to_string(1) + ".png", 
		 Rect(0, 0, 64, 64)
	 ));
	 animFrames.pushBack(SpriteFrame::create(
		 this->_unitType + "/" + std::to_string(dir) + "_" + std::to_string(2) + ".png",
		 Rect(0, 0, 64, 64)
	 ));
	 Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.5f);
	 Animate* animate = Animate::create(animation);
	 
	 
	 this->runAction(RepeatForever::create(animate));
	 
 }

inline void Unit::animate_move_forever_pos(Vec2 pos_target) {
	Vec2 vecDir = pos_target - this->getPosition();
	float angleDir = RTOD(vecDir.getAngle());
	int iDir;
	if (INRANGE(angleDir, -22.5f, 22.5f)) {
		
		iDir = Dir::right;
	}
	else if (INRANGE(angleDir, 22.5f, 67.5f)) {
		iDir = Dir::up_right;
	}
	else if (INRANGE(angleDir, 67.5f, 112.5f)) {
		iDir = Dir::up;
	}
	else if (INRANGE(angleDir, 112.5f, 157.5f)) {
		iDir = Dir::up_left;
	}
	else if (INRANGE(angleDir, 157.5f, 181.f) || INRANGE(angleDir, -181.f, -157.5f)) {
		iDir = Dir::left;
	}
	else if (INRANGE(angleDir, -157.5f, -112.5f)) {
		iDir = Dir::down_left;
	}
	else if (INRANGE(angleDir, -112.5f, -67.5f)) {
		iDir = Dir::down;
	}
	else if (INRANGE(angleDir, -67.5f, -22.5f)) {
		iDir = Dir::down_right;
	}
	this->animate_move_forever(iDir);
}

