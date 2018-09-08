#include "DXUT.h"
#include "Catapult.h"
#include "MultiDirAnimation.h"
#include "World.h"
#include "GameScene.h"

Catapult::Catapult() : delay(20), attacked(false)
{
	collisionPos = Vec2(220 + TILEWIDTH / 2, 346);
	addChild(sprite = new Sprite("image/build/catapult.dds"));
	addChild(animationMap[IDLE] = new MultiDirAnimation(this, "image/build/catapult", 0, 1));
	rect = animationMap[IDLE]->rect;
	animationMap[IDLE]->visible = false;
	addChild(animationMap[ATTACK] = new MultiDirAnimation(this, "image/build/catapult", 13, 6));
	animationMap[ATTACK]->visible = false;
	attackPos = Vec2(220, 220) - collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2);
	//-collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2)
}

Catapult::~Catapult()
{
}

void Catapult::update(float dt)
{
	Building::update(dt);

	GameScene *scene = (GameScene*)world.currentScene;

	if (!attackTarget && sprite->color.a == 1)
	{
		float min = 1300;
		for_iter(cave, scene->caveList)
		{
			float dis = pita(GetCollisionPos(), (*cave)->GetCollisionPos());
			if (min > dis && dis <= 1300)
			{
				min = dis;
				attackTarget = (*cave);
			}
		}
	}

	delay -= dt;

	if (delay <= 0 && attackTarget) {
		delay = 20;
		animationMap[IDLE]->visible = false;
		animationMap[ATTACK]->visible = true;
		((MultiDirAnimation*)animationMap[ATTACK])->Reset();
		moveRotation = angle(GetCollisionPos(), attackTarget->GetCollisionPos());
	}

	MultiDirAnimation *attackAni = (MultiDirAnimation*)animationMap[ATTACK];
	
	if (attackTarget && (int)attackAni->currentFrame == 6 && !attacked && attackAni->visible)
	{
		Arrow *arrow = new Arrow(angle(pos + attackPos, attackTarget->GetCollisionPos()), 500, 3, 70);
		arrow->setCenter(pos + attackPos);
		arrow->rotation += D3DXToRadian(90);
		scene->arrowList.push_back(arrow);
		scene->addChild(arrow);
		attacked = true;
	}
	else if (!((int)attackAni->currentFrame == 6))
		attacked = false;

	if (attackAni->visible && (int)attackAni->currentFrame == attackAni->animation[0]->textures.size() - 1)
	{
		attackAni->visible = false;
		animationMap[IDLE]->visible = true;
	}
}