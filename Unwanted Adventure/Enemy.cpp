#include "DXUT.h"
#include "Enemy.h"
#include "MultiDirAnimation.h"
#include "GameScene.h"
#include "World.h"

Enemy::Enemy() : scanRange(0), moveRange(0), attackRange(0), attackFin(false)
{
}

Enemy::~Enemy()
{
}

Deer::Deer()
{
	type = DEER;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/monster/deer", 12, 10));
	rect = animationMap[WALK]->rect;
	addChild(animationMap[IDLE] = new MultiDirAnimation(this, "image/monster/deer", 12, 10));
	animationMap[IDLE]->visible = false;
	rect.cutOff(Rect(60, 65, 60, 60));

	moveRange = 300;
	moveSpeed = 100;
	collisionPos = Vec2(172, 205);
	colRange = 40;

	maxHp = hp = 30;
	addChild(hpBarBack = new Sprite("image/ui/hpBar/back.png"));
	addChild(hpBar = new Sprite("image/ui/hpBar/front.png"));
	hpBarBack->pos = Vec2(122, 76);
	hpBar->pos = Vec2(124, 78);
}

Deer::~Deer() {}

void Deer::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;

	Enemy::update(dt);
	if (routeList.empty())
	{
		Vec2 walkPos = Vec2(random(GetCollisionPos().x - moveRange, GetCollisionPos().x + moveRange), random(GetCollisionPos().y - moveRange, GetCollisionPos().y + moveRange));
		moveSpeed = 150;

		while (scene->GetTile(walkPos)->code != GROUND_TILE)
			walkPos = Vec2(random(GetCollisionPos().x - moveRange, GetCollisionPos().x + moveRange), random(GetCollisionPos().y - moveRange, GetCollisionPos().y + moveRange));

		SetWalk(walkPos);
	}
}

Boar::Boar() : dash(false), dashDir(0), delay(0)
{
	type = BOAR;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/monster/wildBoar/walk", 8, 8));
	rect = animationMap[WALK]->rect;
	addChild(animationMap[IDLE] = new MultiDirAnimation(this, "image/monster/wildBoar/walk", 8, 8));
	animationMap[IDLE]->visible = false;
	addChild(animationMap[ATTACK] = new MultiDirAnimation(this, "image/monster/wildBoar/attack", 4, 8));
	animationMap[ATTACK]->visible = false;
	rect.cutOff(Rect(60, 65, 60, 60));
	scanRange = 2000;
	attackRange = 300;
	moveRange = 200;
	moveSpeed = 150;
	collisionPos = Vec2(112, 112);
	colRange = 40;

	maxHp = hp = 60;
	addChild(hpBarBack = new Sprite("image/ui/hpBar/back.png"));
	addChild(hpBar = new Sprite("image/ui/hpBar/front.png"));
	hpBarBack->pos = Vec2(62, 40);
	hpBar->pos = Vec2(64, 42);
}

Boar::~Boar() { }

void Boar::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;

	Enemy::update(dt);

	if (delay > 0)
		delay -= dt;

	if (dash)
	{ 
		if (circleCollision(GetCollisionPos(), scene->player->GetCollisionPos(), colRange + scene->player->colRange + 10))
		{
			scene->player->moveRotation = angle(scene->player->GetCollisionPos(), GetCollisionPos());
			scene->player->Attacked(dashDir, 350);
			scene->player->routeList.clear();
			moveSpeed = 150;
			dash = false;
			canMove = true;
			delay = 2;
		}

		moveRotation = dashDir;
		pos += Vec2(cos(dashDir), sin(dashDir)) * moveSpeed * dt;
		ChangeAnimation(ATTACK);
		if (currentTile->code == DEEP_WATER_TILE)
		{
			moveSpeed = 150;
			dash = false;
			canMove = true;
		}
	}
	else
	{
		if (pita(scene->player->GetCollisionPos(), GetCollisionPos()) < attackRange && !dash && delay <= 0)
		{
			routeList.clear();
			moveRotation = dashDir = angle(GetCollisionPos(), scene->player->GetCollisionPos());
			moveSpeed = 250;
			dash = true;
			canMove = false;

			dashTimer.reset(3, 1);
			dashTimer.onFinished = [=](){
				moveSpeed = 150;
				dash = false;
				canMove = true;
			};
		}
		else if (pita(scene->player->GetCollisionPos(), GetCollisionPos()) < scanRange && routeList.empty() && delay <= 0)
		{
			SetWalk(scene->player->GetCollisionPos());
		}
		else if (routeList.empty())
		{
			Vec2 walkPos = Vec2(random(GetCollisionPos().x - moveRange, GetCollisionPos().x + moveRange), random(GetCollisionPos().y - moveRange, GetCollisionPos().y + moveRange));
			moveSpeed = 150;
			SetWalk(walkPos);
		}
	}
}

Snake::Snake()
{
	type = SNAKE;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/monster/snake/walk", 6, 6));
	rect = animationMap[WALK]->rect;														 
	addChild(animationMap[IDLE] = new MultiDirAnimation(this, "image/monster/snake/walk", 6, 6));
	animationMap[IDLE]->visible = false;
	addChild(animationMap[ATTACK] = new MultiDirAnimation(this, "image/monster/snake/attack", 8, 8));
	animationMap[ATTACK]->visible = false;
	rect.cutOff(Rect(60, 65, 60, 60));
	scanRange = 300;
	attackRange = 180;
	moveRange = 100;
	moveSpeed = 100;
	collisionPos = Vec2(100, 100);
	colRange = 20;
	attackFrame = 5;

	maxHp = hp = 30;
	addChild(hpBarBack = new Sprite("image/ui/hpBar/back.png"));
	addChild(hpBar = new Sprite("image/ui/hpBar/front.png"));
	hpBarBack->pos = Vec2(50, 37);
	hpBar->pos = Vec2(52, 39);
}

Snake::~Snake() { }

void Snake::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;

	Enemy::update(dt);

	if (pita(scene->player->GetCollisionPos(), GetCollisionPos()) < attackRange)
	{
		attackTarget = scene->player;
		ChangeAnimation(ATTACK);
		routeList.clear();
		moveRotation = angle(GetCollisionPos(), scene->player->GetCollisionPos());
	}
	else if (pita(scene->player->GetCollisionPos(), GetCollisionPos()) < scanRange && routeList.empty())
	{
		SetWalk(scene->player->GetCollisionPos());
	}
	else if (routeList.empty())
	{
		Vec2 walkPos = Vec2(random(GetCollisionPos().x - moveRange, GetCollisionPos().x + moveRange), random(GetCollisionPos().y - moveRange, GetCollisionPos().y + moveRange));
		
		SetWalk(walkPos);
	}

	MultiDirAnimation *attackAni = (MultiDirAnimation*)animationMap[ATTACK];

	if (attackTarget && (int)attackAni->currentFrame == attackFrame && !attacked)
	{
		scene->player->Attacked(angle(GetCollisionPos(), attackTarget->GetCollisionPos()), 80);
		attacked = true;
	}
	else if (!((int)attackAni->currentFrame == attackFrame))
		attacked = false;
}

Eagle::Eagle() : delay(0)
{
	type = EAGLE;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/monster/eagle/walk", 6, 5));
	rect = animationMap[WALK]->rect;
	addChild(animationMap[IDLE] = new MultiDirAnimation(this, "image/monster/eagle/walk", 6, 7));
	animationMap[IDLE]->visible = false;
	addChild(animationMap[ATTACK] = new MultiDirAnimation(this, "image/monster/eagle/attack", 5, 5));
	animationMap[ATTACK]->visible = false;
	rect.cutOff(Rect(75, 75, 75, 75));
	scanRange = 800;
	attackRange = 200;
	moveRange = 300;
	moveSpeed = 200;
	collisionPos = Vec2(186, 186);
	colRange = 40;
	attackFrame = 2;

	maxHp = hp = 50;
	addChild(hpBarBack = new Sprite("image/ui/hpBar/back.png"));
	addChild(hpBar = new Sprite("image/ui/hpBar/front.png"));
	hpBarBack->pos = Vec2(136, 106);
	hpBar->pos = Vec2(138, 108);
}

Eagle::~Eagle() { }

void Eagle::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;

	Enemy::update(dt);

	if (delay > 0)
		delay -= dt;

	if (pita(scene->player->GetCollisionPos(), GetCollisionPos()) < attackRange && delay <= 0)
	{
		attackTarget = scene->player;
		routeList.clear();
		ChangeAnimation(ATTACK);
		moveRotation = angle(GetCollisionPos(), scene->player->GetCollisionPos());
	}
	else if (pita(scene->player->GetCollisionPos(), GetCollisionPos()) < scanRange && routeList.empty() && delay <= 0 && attackFin)
	{
		SetWalk(scene->player->GetCollisionPos());
	}
	else if (routeList.empty() && attackFin)
	{
		Vec2 walkPos = Vec2(random(GetCollisionPos().x - moveRange, GetCollisionPos().x + moveRange), random(GetCollisionPos().y - moveRange, GetCollisionPos().y + moveRange));
	
		SetWalk(walkPos);
	}

	MultiDirAnimation *attackAni = (MultiDirAnimation*)animationMap[ATTACK];

	if (attackTarget && (int)attackAni->currentFrame == attackFrame && !attacked)
	{
		delay = 20;
		scene->player->Attacked(angle(GetCollisionPos(), attackTarget->GetCollisionPos()), 400, 5);
		attacked = true;
	}
	else if (!((int)attackAni->currentFrame == attackFrame))
		attacked = false;

	if (attackAni->visible && attackAni->currentFrame < attackAni->animation[0]->textures.size() - 1)
		attackFin = false;
	else
		attackFin = true;
}

Scorpion::Scorpion()
{
	type = SCORPION;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/monster/scorpion/walk", 12, 10));
	rect = animationMap[WALK]->rect;
	addChild(animationMap[IDLE] = new MultiDirAnimation(this, "image/monster/scorpion/walk", 12, 10));
	animationMap[IDLE]->visible = false;
	addChild(animationMap[ATTACK] = new MultiDirAnimation(this, "image/monster/scorpion/attack", 8, 8));
	animationMap[ATTACK]->visible = false;
	rect.cutOff(Rect(17, 17, 17, 17));
	scanRange = 300;
	attackRange = 100;
	moveRange = 150;
	moveSpeed = 100;
	collisionPos = Vec2(90, 90);
	colRange = 20;
	attackFrame = 6;

	maxHp = hp = 40;
	addChild(hpBarBack = new Sprite("image/ui/hpBar/back.png"));
	addChild(hpBar = new Sprite("image/ui/hpBar/front.png"));
	hpBarBack->pos = Vec2(40, 34);
	hpBar->pos = Vec2(42, 36);
}

Scorpion::~Scorpion() { }

void Scorpion::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;

	Enemy::update(dt);

	if (pita(scene->player->GetCollisionPos(), GetCollisionPos()) < attackRange)
	{
		attackTarget = scene->player;
		ChangeAnimation(ATTACK);
		routeList.clear();
		moveRotation = angle(GetCollisionPos(), scene->player->GetCollisionPos());
	}
	else if (pita(scene->player->GetCollisionPos(), GetCollisionPos()) < scanRange && routeList.empty())
	{
		SetWalk(scene->player->GetCollisionPos());
	}
	else if (routeList.empty())
	{
		Vec2 walkPos = Vec2(random(GetCollisionPos().x - moveRange, GetCollisionPos().x + moveRange), random(GetCollisionPos().y - moveRange, GetCollisionPos().y + moveRange));
		SetWalk(walkPos);
	}

	MultiDirAnimation *attackAni = (MultiDirAnimation*)animationMap[ATTACK];

	if (attackTarget && (int)attackAni->currentFrame == attackFrame && !attacked)
	{
		scene->player->Attacked(angle(GetCollisionPos(), attackTarget->GetCollisionPos()), 40, 5);
		attacked = true;
	}
	else if (!((int)attackAni->currentFrame == attackFrame))
		attacked = false;
}
