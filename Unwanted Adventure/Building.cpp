#include "DXUT.h"
#include "Building.h"
#include "GameScene.h"
#include "World.h"'
#include "Catapult.h"

Building::Building() : buildTile(nullptr), buildMode(true), canBuild(false), sprite(nullptr), animation(nullptr), buildingTimer(0), dustTimer(0), fade(1)
{
}

Building::~Building()
{
}

void Building::update(float dt)
{
	Object::update(dt);
	GameScene* scene = (GameScene*)world.currentScene;

	Vec2 mousePos = world.getMousePos() - scene->pos;

	if (buildMode)
	{
		sprite->pos = -collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2);
		sprite->color.a = 0.5f;
		buildTile = scene->GetTile(mousePos);
		pos = buildTile->pos;
		
		CheckCanBuild();
	}
}

void Building::CheckCanBuild() 
{
	GameScene *scene = (GameScene*)world.currentScene;
	
	int cnt = 0;

	for_iter(check, checkList)
	{
		Tile *tile = scene->GetTile(pos + (*check)->center());
		if (tile->blocked || tile->unitTile || tile->buildTile || !tile->canBuild || tile->toolTile)
		{
			(*check)->setPath("image/map/red.png");
			cnt++;
		}
		else (*check)->setPath("image/map/green.png");
	}

	if (cnt == 0) canBuild = true;
	else canBuild = false;
}

void Building::CheckCanBuild(TILE_CODE code)
{
	GameScene *scene = (GameScene*)world.currentScene;

	int cnt = 0;

	for_iter(check, checkList)
	{
		Tile *tile = scene->GetTile(pos + (*check)->center());

		if (tile->blocked || tile->unitTile || tile->buildTile || !tile->canBuild || tile->toolTile || tile->code != code)
		{
			(*check)->setPath("image/map/red.png");
			cnt++;
		}
		else (*check)->setPath("image/map/green.png");
	}

	if (cnt == 0) canBuild = true;
	else canBuild = false;
}

void Building::instantBuild()
{
	GameScene *scene = (GameScene*)world.currentScene;

	currentTile = buildTile;
	collisionPos = Vec2(TILEWIDTH / 2 + (width - 1) * TILEWIDTH / 4 + (height - 1) * TILEWIDTH / 4,
		              TILEHEIGHT / 2 - (width - 1) * TILEHEIGHT / 4 + (height - 1) * TILEHEIGHT / 4);

	for_iter(iter, checkList)
	{
		scene->GetTile(pos + (*iter)->center())->buildTile = true;
		(*iter)->visible = false;
	}
	buildMode = false;

	scene->player->SetWalk(currentTile->GetCenter());
	scene->player->canMove = false;
	scene->player->isBuilding = true;
	scene->player->building = this;

	sprite->color.a = 0.5f;
}

void Building::onDead()
{
	GameScene *scene = (GameScene*)world.currentScene;
	for_iter(iter, scene->objList)
	{
		if ((*iter)->attackTarget == this)
			(*iter)->attackTarget = nullptr;
	}
	for_iter(iter, checkList)
	{
		scene->GetTile(pos + (*iter)->center())->buildTile = false;
		removeChild(*iter);
	}

	FadeOut();
}

void Building::setCheckTile()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Sprite *t;
			t = new Sprite("image/map/green.png");
			t->pos = Vec2(x * TILEWIDTH / 2 + y * TILEWIDTH / 2, -x * TILEHEIGHT / 2 + y * TILEHEIGHT / 2);
			t->color.a = 0.5f;
			addChild(t);
			checkList.push_back(t);
		}
	}
	buildMode = true;
}

void Building::FadeOut()
{
	GameScene *scene = (GameScene*)world.currentScene;
	fadeOutTimer.reset(0.01f, 25);
	fadeOutTimer.onTick = [=]()
	{
		fade -= 0.04f;
		if (animation)
			animation->color = D3DXCOLOR(1, 1, 1, fade);
		if (sprite)
			sprite->color = D3DXCOLOR(1, 1, 1, fade);
	};
	fadeOutTimer.onFinished = [=](){
		removeChild(this);
		scene->objList.remove(this); 
		for_iter(iter, scene->objList)
		{
			if ((*iter)->attackTarget == this)
				(*iter)->attackTarget = nullptr;
		}
		if (type == CAVE)
			scene->caveList.remove((Cave*)this);
	};
}

void Building::Build(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;

	buildingTimer += dt;
	dustTimer += dt;

	sprite->color.a = 0.5f + buildingTimer / 12;

	if (dustTimer > 0.2f && buildingTimer < 4.2f)
	{
		dustTimer = 0;
		Sprite *dust = new Sprite("image/dust.png");
		scene->addChild(dust);
		dust->color.a = 0.2f;
		dust->rotationCenter = dust->rect.center();
		dust->scaleCenter = dust->rect.center();
		dust->setCenter(GetCollisionPos() + (Vec2(random(-TILEWIDTH, TILEWIDTH), random(-TILEHEIGHT, TILEHEIGHT)) / 2));
		scene->dustList.push_back(dust);
	}

	if (buildingTimer > 3)
	{
		sprite->color.a = 1;
		scene->player->canMove = true;
		scene->player->isBuilding = false;
		scene->player->building = nullptr;

		if (type == HOME)
			scene->ui->buildHome = true;
		else if (type == WELL)
			scene->ui->buildWell = true;
		else if (type == WORKROOM)
			scene->ui->buildWorkRoom = true;
		else if (type == FORGE)
			scene->ui->buildForge = true;

		for (int i = 0; i < 8; i++)
		{
			Particle *p = new Particle("image/star.png", D3DXToRadian(360) / 8 * i, 500);
			p->setCenter(GetCollisionPos());
			scene->addChild(p);
			scene->particleList.push_back(p);
		}

		if (animation)
		{
			sprite->visible = false;
			animation->visible = true;
			animation->pos = sprite->pos;
		}
		if (type == CATAPULT)
		{
			sprite->visible = false;
			((Catapult*)this)->animationMap[IDLE]->pos = sprite->pos;
			((Catapult*)this)->animationMap[ATTACK]->pos = sprite->pos;
			cout << "Build Fin " << endl;
			((Catapult*)this)->animationMap[IDLE]->visible = true;
		}
	}
}