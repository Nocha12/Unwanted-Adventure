#include "DXUT.h"
#include "BuildingFactory.h"
#include "GameScene.h"
#include "World.h"
#include "Cave.h"
#include "Tree.h"
#include "Catapult.h"

BuildingFactory::BuildingFactory(Scene *scene) : gscene(scene)
{
}

BuildingFactory::~BuildingFactory()
{
}

Building* BuildingFactory::MakeBuilding(ObjectType type, bool buildNow, bool spawnSide, TILE_CODE code, bool isCave)
{
	GameScene *scene = (GameScene*)gscene;

	if (type == HOME)
	{
		Building *b = new Building();
		b->type = HOME;
		b->addChild(b->sprite = new Sprite("image/build/house.png"));
		b->rect = b->sprite->rect;
		b->width = 2;
		b->height = 2;
		b->setCheckTile();
		scene->waitBuild = b;

		b->colRange = TILEHEIGHT / 2;
		b->collisionPos = Vec2(302, 380);

		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		b->rect.cutOff(Rect(159, 280, 155, 96));

		if (!buildNow)
			return b;
	}

	if (type == CAVE)
	{
		Building *b = new Cave();
		b->type = CAVE;
		b->width = 2;
		b->height = 2;

		b->setCheckTile();
		scene->waitBuild = b;

		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		b->rect.cutOff(Rect(33, 43, 42, 31));

		if (!buildNow)
			return b;
	}

	if (type == TREE)
	{
		Building *b = new Tree();
		b->type = TREE;
		b->rect = b->sprite->rect;
		b->width = 1; 
		b->height = 1;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 10;
		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		b->rect.cutOff(Rect(100, 50, 100, 100));
		if (!buildNow)
			return b;
	}

	if (type == CATAPULT)
	{
		Building *b = new Catapult();
		b->type = CATAPULT;
		b->rect = b->sprite->rect;
		b->width = 1;
		b->height = 1;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 10;
		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		if (!buildNow)
			return b;
	}

	if (type == RAFT)
	{
		Building *b = new Building();
		b->type = RAFT;
		b->addChild(b->sprite = new Sprite("image/raft.png"));
		b->rect = b->sprite->rect;
		b->width = 1;
		b->height = 1;
		b->setCheckTile();
		scene->waitBuild = b;

		b->collisionPos = Vec2(157 + TILEWIDTH / 2, 160);

		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		b->rect.cutOff(Rect(83, 112, 68, 88));

		if (!buildNow)
			return b;
	}

	if (type == ROCK)
	{
		Building *b = new Building();
		b->type = ROCK;
		b->addChild(b->sprite = new Sprite("image/build/rock.png"));
		b->rect = b->sprite->rect;
		b->width = 2;
		b->height = 2;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 10;
		b->collisionPos = Vec2(106, 111);
		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		b->rect.cutOff(Rect(33, 37, 15, 35));

		if (!buildNow)
			return b;
	}

	if (type == WELL)
	{
		Building *b = new Building();
		b->type = WELL;
		b->addChild(b->sprite = new Sprite("image/build/well.png"));
		b->rect = b->sprite->rect;
		b->width = 2;
		b->height = 2;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 40;
		b->collisionPos = Vec2(155, 182);

		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		b->rect.cutOff(Rect(67, 75, 73, 63));

		if (!buildNow)
			return b;
	}

	if (type == WORKROOM)
	{
		Building *b = new Building();
		b->type = WORKROOM;
		b->addChild(b->sprite = new Sprite("image/build/make.png"));
		b->rect = b->sprite->rect;
		b->width = 1;
		b->height = 2;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 20;
		b->collisionPos = Vec2(200, 175);

		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		b->rect.cutOff(Rect(53, 38, 55, 44));

		if (!buildNow)
			return b;
	}

	if (type == FORGE)
	{
		Building *b = new Building();
		b->type = FORGE;
		b->addChild(b->sprite = new Sprite("image/build/fortress.png"));
		b->rect = b->sprite->rect;
		b->addChild(b->animation = new Animation("image/build/fortress", 8, 8, true, true));
		b->animation->visible = false;
		b->width = 3;
		b->height = 2;
		b->setCheckTile();
		scene->waitBuild = b;
		b->colRange = 20;
		b->collisionPos = Vec2(120, 220);

		b->rect = b->rect.offset(-b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2));
		b->rect.cutOff(Rect(55, 64, 68, 65));

		if (!buildNow)
			return b;
	}

	if (buildNow)
	{
		Building *b = scene->waitBuild;
		scene->objList.push_back(b);
		b->buildMode = false;
		b->sprite->color.a = 1;
		b->sprite->pos = -b->collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2);
		scene->player->canMove = true;
		scene->player->isBuilding = false;
		scene->player->building = nullptr;
		scene->waitBuild = nullptr;

		while (!b->canBuild)
		{
			if (isCave)
			{
				bool flag = false;
				b->buildTile = scene->GetTile(Vec2(random(scene->raft->pos.x - 2000, scene->raft->pos.x + 2000), random(scene->raft->pos.y - 2000, scene->raft->pos.y + 2000)));
				
				for_iter(cave, scene->caveList)
				{
					if (pita((*cave)->currentTile->pos, b->buildTile->pos) < 400)
						flag = true;
				}
				if (flag)
					continue;
			}
			else
			{
				if (spawnSide)
					b->buildTile = scene->GetRandomSideTile(code);
				else
					b->buildTile = scene->GetRandomTile(code);
			}

			b->pos = b->buildTile->pos;
			b->CheckCanBuild(code);
		}

		b->currentTile = b->buildTile;
		
		b->collisionPos = Vec2(TILEWIDTH / 2 + (b->width - 1) * TILEWIDTH / 4 + (b->height - 1) * TILEWIDTH / 4,
							   TILEHEIGHT / 2 - (b->width - 1) * TILEHEIGHT / 4 + (b->height - 1) * TILEHEIGHT / 4);

		for_iter(iter, b->checkList)
		{
			scene->GetTile(b->pos + (*iter)->center())->buildTile = true;
			(*iter)->visible = false;
		}

		return b;
	}
}
