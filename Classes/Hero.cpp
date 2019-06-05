#include "Hero.h"
#include "skillClass.h"
#include "GameScene.h"

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



void Hero::UsingFireBall(Vec2 newPosition) {
	static clock_t last_time = 0;
	clock_t now_time = clock();
	float pass_time = now_time - last_time;
	if (pass_time < 2000) {// cd时间在这里调整
		log("fireball still cd");
		return;
	}
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
	stopAllActions();
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
	stopAllActions();
	int life_this = _life_current;
	_life_current = enemy->_life_current;
	enemy->_life_current = life_this;
	enemy->getDamaged(this, 0);
	this->getDamaged(enemy, 0);
}