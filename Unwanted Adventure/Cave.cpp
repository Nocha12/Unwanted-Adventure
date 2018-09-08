#include "DXUT.h"
#include "Cave.h"
#include "Enemy.h"
#include "World.h"
#include "GameScene.h"
#include "MultiDirAnimation.h"
#include "GameManager.h"

Cave::Cave() : spawnPos(76, 189), timer(0), isBroken(false)
{
	hp = maxHp = 1000; 
	addChild(sprite = new Sprite("image/build/cave.png"));
	rect = sprite->rect;
	collisionPos = Vec2(120, 128);
	colRange = TILEHEIGHT / 2;
	addChild(hpBarBack = new Sprite("image/ui/hpBar/back.png"));
	addChild(hpBar = new Sprite("image/ui/hpBar/front.png"));
	hpBarBack->pos = Vec2(70, 11) - collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2);
	hpBar->pos = Vec2(72, 13) - collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2);
	hpBarBack->visible = false;
	hpBar->visible = false;
}

Cave::~Cave() { }

void Cave::update(float dt)
{
	Building::update(dt);

	GameScene *scene = (GameScene*)world.currentScene;

	if (gameManager.boarKillCount >= 6)
	{
		hpBarBack->visible = true;
		hpBar->visible = true;
	}
	else
		hp = maxHp;

	if (hp <= 0 && !isBroken)
	{
		isBroken = true;
		onDead();
	}

	timer += dt;
	if (timer >= 8 && scene->ui->buildForge)
	{
		if (gameManager.enemyCount >= 12)
			return;
		 
		ObjectType eType;

		if (gameManager.boarCount < 6)
		{
			gameManager.boarCount++;
			eType = BOAR;
		}
		else if (gameManager.eagleCount < 5)
		{
			gameManager.eagleCount++;
			eType = EAGLE;
		}
		else
			eType = (ObjectType)random(3, 4);

		MakeEnemy(eType);
		gameManager.enemyCount++;
		timer = 0;
	}

	//if (fade != 1)
	//	return;
}

void Cave::MakeEnemy(ObjectType type)
{
	GameScene *scene = (GameScene*)world.currentScene;

	if (type == BOAR)
	{
		Enemy *boar = new Boar();
		boar->SetCollisionPos(scene->GetTile(pos + spawnPos)->GetCenter());
		scene->addChild(boar);
		scene->objList.push_back(boar);
		for_iter(ani, boar->animationMap)
			((MultiDirAnimation*)ani->second)->FaidIn();
	}
	else if (type == SNAKE)
	{
		Enemy *e = new Snake();
		e->SetCollisionPos(scene->GetTile(pos + spawnPos)->GetCenter());
		scene->addChild(e);
		scene->objList.push_back(e);
		for_iter(ani, e->animationMap)
			((MultiDirAnimation*)ani->second)->FaidIn();
	}
	else if (type == EAGLE)
	{
		Enemy *e = new Eagle();
		e->SetCollisionPos(scene->GetTile(pos + spawnPos)->GetCenter());
		scene->addChild(e);
		scene->objList.push_back(e);
		for_iter(ani, e->animationMap)
			((MultiDirAnimation*)ani->second)->FaidIn();
	}
	else if (type == SCORPION)
	{
		Enemy *e = new Scorpion();
		e->SetCollisionPos(scene->GetTile(pos + spawnPos)->GetCenter());
		scene->addChild(e);
		scene->objList.push_back(e);
		for_iter(ani, e->animationMap)
			((MultiDirAnimation*)ani->second)->FaidIn();
	}
}