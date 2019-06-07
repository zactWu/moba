#include "Hero.h"
#include "skillClass.h"
#include "GameScene.h"
#include "GlobalVal.h"
#include "control.h"

USING_NS_CC;
Hero* Hero::create(const std::string& filename, const std::string& unitType,
	int maxLife, int attack, int defense, int speed,
	float rotate_speed, float attackInterval, float attackRange) {

	auto hero = new (std::nothrow) Hero;
	if (hero && hero->initWithFile(filename)) {

		//初始化hero各项属性
		hero->_life_max = maxLife;
		hero->_life_current = maxLife;
		hero->_attack = attack;
		hero->_defense = defense;
		hero->_speed = speed;
		hero->_unitType = unitType;
		hero->_attackInterval = attackInterval;
		hero->_attackRange = attackRange;
		hero->_tag_attackTarget = -1;
		hero->_onAttack = false;
		hero->_kill_award = 300;
		//创建血条
		hero->_lifeBank = Sprite::create("lifeBank.jpg");

		hero->_lifeBank->setPosition(hero->getPosition() + Vec2(30.f, 60.f));
		hero->addChild(hero->_lifeBank, 1);
		hero->autorelease();

		return hero;
	}

	CC_SAFE_DELETE(hero);
	return nullptr;
}

void Hero::heal(int life_heal) {
	_life_current += life_heal;
	if (_life_current > _life_max) {
		_life_current = _life_max;
	}
	_lifeBank->setScaleX(static_cast<float>(_life_current) / _life_max);
}

void Hero::turnTo(Vec2 pos)
{
	int iDir = getDirByTargetPos(pos);
}



void Hero::UsingFireBall(Vec2 newPosition) {
	Wskill_cd_time = 2000;
	if (clock() - Wskill_last_release_time < Wskill_cd_time) {
		return;
	}
	Wskill_last_release_time = clock();

	if (this == NULL) {
		log("???");
		return;
	}
	static clock_t last_time = 0;
	clock_t now_time = clock();
	float pass_time = now_time - last_time;
	last_time = now_time;
	auto skill = Skill::create("fireboll.jpg", 300, 10, 300, 50);
	skill->_skiller = this;
	skill->setScale(0.3);
	skill->setPosition(getPosition());
	skill->_st_pos = getPosition();
	auto gameScene = dynamic_cast<GameScene*>(getParent()->getParent());
	gameScene->skill_map[gameScene->skill_num] = skill;
	gameScene->skill_num++;
	skill->_side = _side;
	skill->_release_time = clock();
	gameScene->map->addChild(skill, 12);//这里有一点问题要解决
	skill->move(skill->_st_pos, newPosition);
}

void Hero::useSkill_trample() {
	Wskill_cd_time = 2000;
	if (clock() - Wskill_last_release_time < Wskill_cd_time) {
		return;
	}
	Wskill_last_release_time = clock();

	stopAllActions();
	specialEffect("trample.png", getPosition());
	auto gameScene = dynamic_cast<GameScene*> (getParent()->getParent());
	auto it = gameScene->unit_map.begin();
	while (it != gameScene->unit_map.end()) {
		auto enemy = it->second;
		if (enemy->_side != _side && enemy->getPosition().distance(getPosition()) < 200) {
			enemy->getDamaged(this, 10);
			enemy->stunned(2.0);
		}
		++it;
	}
}

void Hero::useSkill_blinkToEnemy(Unit* enemy)
{   
	if (enemy == nullptr) {
		return;
	}
	Qskill_cd_time = 2000;
	if (clock() - Qskill_last_release_time < Qskill_cd_time) {
		return;
	}
	Qskill_last_release_time = clock();

	
	stopAllActions();
	auto pos_current = getPosition();
	auto pos_enemy = enemy->getPosition();
	auto diffVec = pos_enemy - pos_current;
	auto diffVec_unt = diffVec / diffVec.length();
	auto pos_target = pos_enemy - 64 * diffVec_unt;
	setPosition(pos_target);
	enemy->getDamaged(this, 20);
}

void Hero::useSkill_ConeWave(const Vec2 &pos_target) {
	Qskill_cd_time = 2000;
	if (clock() - Qskill_last_release_time < Qskill_cd_time) {
		return;
	}
	Qskill_last_release_time = clock();

	stopAllActions();
	auto pos_current = getPosition();
	auto diffVec = pos_target - pos_current;
	auto diffVec_unt = diffVec / diffVec.length();
	auto point1 = pos_current + 64 * diffVec_unt;
	auto point2 = pos_current + 128 * diffVec_unt;
	auto point3 = pos_current + 256 * diffVec_unt;

	auto gameScene = dynamic_cast<GameScene*> (getParent()->getParent());
	auto it = gameScene->unit_map.begin();
	while (it != gameScene->unit_map.end()) {
		auto enemy = it->second;
		auto pos_enemy = enemy->getPosition();
		if (
			enemy->_side != _side && 
			(pos_enemy.distance(point1)<64||
			pos_enemy.distance(point2)<160||
			pos_enemy.distance(point3)<320)
			) 
		{
			enemy->getDamaged(this, 20);
			enemy->stunned(2.0);
			_money += 10;
		}
		++it;
	}
}

void Hero::useSkill_switchLife(Unit* enemy) {
	if (enemy == nullptr) {
		return;
	}
	Eskill_cd_time = 2000;
	if (clock() - Eskill_last_release_time < Eskill_cd_time) {
		return;
	}
	Eskill_last_release_time = clock();

	stopAllActions();
	int life_this = _life_current;
	_life_current = enemy->_life_current;
	enemy->_life_current = life_this;
	enemy->getDamaged(this, 0);
	this->heal(0);
}

void Hero::useSkill_heal()
{
	Wskill_cd_time = 2000;
	if (clock() - Wskill_last_release_time < Wskill_cd_time) {
		return;
	}
	Wskill_last_release_time = clock();

	stopAllActions();
	heal(static_cast<int>(_life_max * 0.3));
}

void Hero::useSkill_ultra(Unit* enemy)
{
	if (enemy == nullptr) {
		return;
	}
	Eskill_cd_time = 2000;
	if (clock() - Eskill_last_release_time < Eskill_cd_time) {
		return;
	}
	Eskill_last_release_time = clock();
	stopAllActions();
	enemy->stunned(2.0);
	enemy->getDamaged(this, static_cast<int>(enemy->_life_max*0.3));
}

void Hero::useSkill_percentageDamage(Unit* enemy) {
	if (enemy == nullptr) {
		return;
	}
	Qskill_cd_time = 2000;
	if (clock() - Qskill_last_release_time < Qskill_cd_time) {
		return;
	}
	Qskill_last_release_time = clock();
	stopAllActions();
	enemy->getDamaged(this, static_cast<int>(enemy->_life_current * 0.15));
}

void Hero::useSkill_tornado()
{
	Eskill_cd_time = 2000;
	if (clock() - Eskill_last_release_time < Eskill_cd_time) {
		return;
	}
	Eskill_last_release_time = clock();
	stopAllActions();
}


void Hero::useSkill(int heroID, int kind, Vec2 pos, int tag) {
	Unit* enemy = NULL;
	if (tag >= 0) {
		enemy = dynamic_cast<Unit*>(getParent()->getChildByTag(tag));
	}
	switch (heroID) {
	case WARRIOR:
		switch (kind) {
		case SKILLQ:
			useSkill_blinkToEnemy(enemy);
			break;
		case SKILLW:
			useSkill_heal();
			break;
		case SKILLE:
			useSkill_ultra(enemy);
			break;
		}
		break;
	case ARCHER:
		switch (kind) {
		case SKILLQ:
			useSkill_ConeWave(pos);
			break;
		case SKILLW:
			UsingFireBall(pos);
			break;
		case SKILLE:
			useSkill_switchLife(enemy);
			break;
		}
		break;
	case MAGICIAN:
		switch (kind) {
		case SKILLQ:
			useSkill_percentageDamage(enemy);
			break;
		case SKILLW:
			useSkill_trample();
			break;
		case SKILLE:
			useSkill_tornado();
			break;
		}
		break;
	}
}

void Hero::specialEffect(const std::string& filename, Vec2 pos)
{
	auto effect = Sprite::create(filename);
	effect->setPosition(pos);
	auto gameMap = getParent();
	gameMap->addChild(effect);
	auto fade = FadeOut::create(1.5f);
	auto cf = CallFunc::create([=]() {
		gameMap->removeChild(effect);
		});
	auto seq = Sequence::create(DelayTime::create(1.5f), cf, nullptr);

	effect->runAction(fade);
	gameMap->runAction(seq);
}