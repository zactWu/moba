#include "UnitClass.h"
#include "cocos2d.h"
#include "helper.h"
#include "skillClass.h"
#include "GameScene.h"
#include "MoveFind.h"
#include "control.h"
#include "GlobalVal.h"
USING_NS_CC;

//Unit的create函数，调用：myUnit = Unit::create(...)
Unit* Unit::create(const std::string& filename, const std::string& unitType,
	int maxLife, int attack, int defense, int speed,
	float rotate_speed, float attackInterval, float attackRange) {

	auto unit = new (std::nothrow) Unit;
	if (unit && unit->initWithFile(filename)) {

		//初始化unit各项属性
		unit->_life_max = maxLife;
		unit->_life_current = maxLife;
		unit->_attack = attack;
		unit->_defense = defense;
		unit->_speed = speed;
		unit->_unitType = unitType;
		unit->_attackInterval = attackInterval;
		unit->_attackRange = attackRange+50;
		unit->_tag_attackTarget = -1;
		unit->_onAttack = false;
		unit->_kill_award = 100;
		//创建血条
		unit->_lifeBank = Sprite::create("lifeBank.jpg");

		unit->_lifeBank->setPosition(unit->getPosition() + Vec2(30.f, 60.f));
		unit->addChild(unit->_lifeBank, 1);
		unit->autorelease();

		return unit;
	}

	CC_SAFE_DELETE(unit);
	return nullptr;
}

//自动判断是否需要停止当前动画并改变移动动画、修改_moveDir
inline void Unit::animate_move_forever(int dir) {
	if (_moveDir != dir) {
		stopActionByTag(Tag::animate_move);
		_moveDir = dir;
		Vector<SpriteFrame*> animFrames;
		animFrames.reserve(2);
		animFrames.pushBack(SpriteFrame::create(
			_unitType + "/" + std::to_string(dir) + "_" + std::to_string(1) + ".png",
			Rect(0, 0, 64, 64)
		));
		animFrames.pushBack(SpriteFrame::create(
			_unitType + "/" + std::to_string(dir) + "_" + std::to_string(2) + ".png",
			Rect(0, 0, 64, 64)
		));
		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.5f);
		Animate* animate = Animate::create(animation);
		auto animate_move = RepeatForever::create(animate);
		animate_move->setTag(Tag::animate_move);
		runAction(animate_move);

	}

}

inline int Unit::getDirByTargetPos(const Vec2& pos_target)const {
	return Unit::getDirByTargetPos(getPosition(), pos_target);
}

inline int Unit::getDirByTargetPos(const Vec2& pos_current, const Vec2& pos_target)const {
	Vec2 vecDir = pos_target - pos_current;
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
	return iDir;
}

//自动停止当前移动、修改_pos_moveTarget和_moveDir。
void Unit::moveTo_directly(const Vec2& pos_target) {
	_pos_moveTarget = pos_target;
	stopActionByTag(Tag::move);
	float distance = pos_target.distance(getPosition());
	float duration = distance / _speed;
	auto move = MoveTo::create(duration, pos_target);
	auto cf_stopAnimation = CallFunc::create([=]() {
		stopActionByTag(Tag::animate_move);
		});

	auto seq_moveThenStopAnimation = Sequence::create(move, cf_stopAnimation, nullptr);
	seq_moveThenStopAnimation->setTag(Tag::move);
	runAction(seq_moveThenStopAnimation);


	int iDir = getDirByTargetPos(pos_target);
	animate_move_forever(iDir);

}

void Unit::moveTo_directly(const std::vector<Vec2> pos_list) {
	if (pos_list.size() == 1) {
		return;
	}
	stopActionByTag(Tag::move);
	std::vector<Vec2> new_pos_list = pos_list;
	new_pos_list.insert(new_pos_list.begin(), this->getPosition());
	cocos2d::Vector<FiniteTimeAction*> actionListToOnePoint;
	std::vector<Vec2>::const_iterator it;
	for (it = new_pos_list.begin(); it != new_pos_list.end() - 1; ++it) {
		Vec2 pos_current = *it;
		Vec2 pos_target = *(it + 1);
		//移动动作
		float distance = pos_current.distance(pos_target);
		float duration = distance / _speed;
		auto move = MoveTo::create(duration, pos_target);
		//
		int iDir = getDirByTargetPos(pos_current, pos_target);
		auto cf = CallFunc::create([=]()mutable {
			this->_pos_moveTarget = pos_target;
			this->animate_move_forever(iDir);

			});
		actionListToOnePoint.pushBack(cf);
		actionListToOnePoint.pushBack(move);
	}
	auto cf_stopAnimate = CallFunc::create([=]()mutable {
		this->stopActionByTag(Tag::animate_move);
		});
	actionListToOnePoint.pushBack(cf_stopAnimate);
	auto actionList = Sequence::create(actionListToOnePoint);
	actionList->setTag(Tag::move);
	runAction(actionList);
}

void Unit::attack_once(Unit* sp_enemy) {

	if (sp_enemy == nullptr) {
		return;
	}
	int iDir = getDirByTargetPos(sp_enemy->getPosition());

	auto cf_onAttack = CallFunc::create([=]() {
		_onAttack = true;
		});


	Vector<SpriteFrame*> animFrames1;
	animFrames1.reserve(1);
	animFrames1.pushBack(SpriteFrame::create(
		_unitType + "/" + std::to_string(iDir) + ".png",
		Rect(0, 0, 64, 64)
	));
	Animation* animation1 = Animation::createWithSpriteFrames(animFrames1, _attackInterval / 2.f);
	Animate* animate1 = Animate::create(animation1);

	auto cf_stopIfEnemyIsDead = CallFunc::create([=]() {
		if (sp_enemy == nullptr) {
			this->_onAttack = false;
			return;
		}
		});

	Vector<SpriteFrame*> animFrames2;
	animFrames2.reserve(1);
	animFrames2.pushBack(SpriteFrame::create(
		_unitType + "/" + std::to_string(iDir) + "a.png",
		Rect(0, 0, 64, 64)
	));
	Animation* animation2 = Animation::createWithSpriteFrames(animFrames2, _attackInterval / 2.f);
	Animate* animate2 = Animate::create(animation2);





	auto cf_damage = CallFunc::create([=]() {
		if (sp_enemy == nullptr) {
			this->_onAttack = false;
			return;
		}
		longRangeAttack(sp_enemy);
		});






	Vector<SpriteFrame*> animFrames3;
	animFrames3.reserve(1);
	animFrames3.pushBack(SpriteFrame::create(
		_unitType + "/" + std::to_string(iDir) + ".png",
		Rect(0, 0, 64, 64)
	));
	Animation* animation3 = Animation::createWithSpriteFrames(animFrames3, 0.05);
	Animate* animate3 = Animate::create(animation3);

	auto cf_notOnAttack = CallFunc::create([=]() {
		this->_onAttack = false;
		});

	auto seq_oneAttack = Sequence::create(cf_onAttack, animate1, cf_stopIfEnemyIsDead,
		animate2, cf_damage, animate3, cf_notOnAttack, nullptr);

	runAction(seq_oneAttack);

	_onAttack = false;
}

//使用时需要修改 unit->_tag_attackTarget 至目标单位。
//使用时采用scene->schedule(update_follow_attack)激活追踪攻击状态，
//取消攻击时需调用unschedule，来节省开销，并且需要修改_attackTarget=-1, _onAttack = false

void Unit::update_follow_attack(float dt) {
	if (_stunned) {
		return;
	}
	//出错处理：无攻击目标
	if (_tag_attackTarget == -1) {
		//log("NO TAR");
		return;
	}
	
	auto sceneMap = getParent();
	auto sp_target = sceneMap->getChildByTag(_tag_attackTarget);
	
	if (sp_target == nullptr) {
		return;
	}
	//log("try to att");
	auto sp_enemy = dynamic_cast<Unit*>(sp_target);

	float distance = getPosition().distance(sp_target->getPosition());
	if (distance > _attackRange) {
		//log("not in range");
		//auto gameScene = dynamic_cast<GameScene*>(getParent()->getParent());
		//moveTo_directly(gameScene->MoveFind(this->getPosition(),sp_target->getPosition()));  //考虑寻路
		//moveTo_directly( sp_target->getPosition());
	}
	else {
		if (!_onAttack) {
			//debug
			stopAllActions();//这个不知道需不需要加
			//log("shoulde att");
			attack_once(sp_enemy);
		}
	}

}

void Unit::getDamaged(Unit* producer, int damage) {
	if (damage < 0) {
		return;
	}
	_life_current -= damage;
	
	if (producer == nullptr) {
		log("warning: this hit has no one declear responsable");
	}
	else {
		_last_attacker = producer;
	}
	
	if (_life_current <= 0) {
	    // 赏金还是放在外面
		return;
	}
	_lifeBank->setScaleX(static_cast<float>(_life_current) / _life_max);
	wound = 1;
	//变红动画
	/*
	auto cf_intoRed = CallFunc::create([=]() {
		if(this!=NULL)
			this->setColor(cocos2d::Color3B::RED);
		});
	auto cf_back = CallFunc::create([=]() {
		if(this!=NULL)
			this->setColor(cocos2d::Color3B(255, 255, 255));
		});
	auto switchColor = Sequence::create(cf_intoRed, DelayTime::create(0.5), cf_back, nullptr);
	getParent()->runAction(switchColor);
	*/
}

void Unit::longRangeAttack(Unit* enemy) {
	if (_stunned) {
		return;
	}
	if (enemy == nullptr) {
		return;
	}

	auto skill = Skill::create("fireboll.jpg", 300, _attack, 500, 50);
	skill->targe = enemy;
	skill->_skiller = this;
	skill->setScale(0.3);
	skill->setPosition(getPosition());
	skill->_st_pos = getPosition();
	auto gameScene = dynamic_cast<GameScene*>(getParent()->getParent());
	gameScene->skill_map[gameScene->skill_num] = skill;
	gameScene->skill_num++;
	skill->_side = _side;
	skill->_release_time = clock();
	gameScene->map->addChild(skill);
}

void Unit::stunned(double duration) {
	//debug
	cocos2d::log("stunned!");

	auto cf_stun = CallFunc::create([=]() {
		if (this!=nullptr)
			this->_stunned = true;
	    });
	auto cf_deStun = CallFunc::create([=]() {
		if (this!=nullptr)
			this->_stunned = false;;
		});
	auto seq = Sequence::create(cf_stun, DelayTime::create(duration), cf_deStun, nullptr);
	getParent()->runAction(seq);
}

void Unit::getDamaged() {
	if (!wound) {
		return;
	}
	auto cf_intoRed = CallFunc::create([=]() {
		if (this!= NULL)
			setColor(cocos2d::Color3B::RED);
		});
	auto cf_back = CallFunc::create([=]() {
		if (this != NULL)
			setColor(cocos2d::Color3B(255, 255, 255));
		});
	auto switchColor = Sequence::create(cf_intoRed, DelayTime::create(0.15), cf_back, nullptr);
	if (getParent() != NULL) {
		getParent()->runAction(switchColor);
	}
	wound = 0;
}

void Unit::GetOrder() {
	GameScene* gs = dynamic_cast<GameScene*>(getParent()->getParent());
	if (gs == NULL && gs->map == NULL) {
		log("sth wrrong!!!!!!!!");
		return;
	}
	for (auto i = order_list.begin(); i != order_list.end();) {
		if (i->kind == MOVEORDER) {
			_tag_attackTarget = i->tag;
			if (i->tag == -1) {// 这个效果你们自己取舍
				_onAttack = 0;
			}
			moveTo_directly(gs->MoveFind(getPosition(), i->pos));
			//log("unit move");
		}
		if (i->kind != 1) {
			// 假定这个是英雄
			_onAttack = 0;
			if (gs->hero == this) {
				gs->hero->useSkill(en_hero_id, i->kind, i->pos, i->tag);
			}
			else if (gs->en_hero == this) {
				log("realesse!!!!!!!!!!");
				gs->en_hero->useSkill(hero_id, i->kind, i->pos, i->tag);
			}
		}
		i = order_list.erase(i);
	}
}