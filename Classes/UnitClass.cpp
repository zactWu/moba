#include "UnitClass.h"
#include "cocos2d.h"
#include "helper.h"
USING_NS_CC;

//Unit��create���������ã�myUnit = Unit::create(...)
Unit* Unit::create(const std::string& filename, const std::string& unitType,
	int maxLife, int attack, int defense, int speed,
	float rotate_speed, float attackInterval) {

	auto unit = new (std::nothrow) Unit;
	if (unit && unit->initWithFile(filename)) {

		//��ʼ��unit��������
		unit->_life_max = maxLife;
		unit->_life_current = maxLife;
		unit->_attack = attack;
		unit->_defense = defense;
		unit->_speed = speed;
		unit->_unitType = unitType;
		unit->_attackInterval = attackInterval;

		unit->autorelease();
		return unit;
	}

	CC_SAFE_DELETE(unit);
	return nullptr;
}

//�Զ��ж��Ƿ���Ҫֹͣ��ǰ�������ı��ƶ��������޸�_moveDir
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

//�Զ�ֹͣ��ǰ�ƶ����޸�_pos_moveTarget��_moveDir��
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
		//�ƶ�����
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

void Unit::attack_once(int iDir, int tag_enemy) {
	_onAttack = true;


	Vector<SpriteFrame*> animFrames1;
	animFrames1.reserve(1);
	animFrames1.pushBack(SpriteFrame::create(
		_unitType + "/" + std::to_string(iDir) + ".png",
		Rect(0, 0, 64, 64)
	));
	Animation* animation1 = Animation::createWithSpriteFrames(animFrames1, _attackInterval / 2.f);
	Animate * animate1 = Animate::create(animation1);

	auto cf_stopIfEnemyIsDead = CallFunc::create([=]() {
		auto enemy_ptr = getParent()->getChildByTag(tag_enemy);
		if (enemy_ptr == nullptr) {
			return;
		}
		});

	Vector<SpriteFrame*> animFrames2;
	animFrames2.reserve(1);
	animFrames2.pushBack(SpriteFrame::create(
		_unitType + "/" + std::to_string(iDir) + "a.png",
		Rect(0, 0, 64, 64)
	));
	Animation * animation2 = Animation::createWithSpriteFrames(animFrames2, _attackInterval / 2.f);
	Animate * animate2 = Animate::create(animation2);

	auto cf = CallFunc::create([=]() {
		auto enemy = getParent()->getChildByTag(tag_enemy);
		auto enemy_unit = static_cast<Unit*>(enemy);

		});

	Vector<SpriteFrame*> animFrames3;
	animFrames3.reserve(1);
	animFrames3.pushBack(SpriteFrame::create(
		_unitType + "/" + std::to_string(iDir) + ".png",
		Rect(0, 0, 64, 64)
	));
	Animation * animation3 = Animation::createWithSpriteFrames(animFrames3, 0.05);
	Animate * animate3 = Animate::create(animation3);

	auto seq_oneAttack = Sequence::create(animate1, cf_stopIfEnemyIsDead,
		animate2, animate3, nullptr);

	_onAttack = false;
}
