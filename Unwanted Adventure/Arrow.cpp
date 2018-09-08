#include "DXUT.h"
#include "Arrow.h"
#include "GameScene.h"
#include "World.h"

Arrow::Arrow(float dir, float speed, float timer, int damage) : dir(dir), speed(speed), timer(timer), damage(damage)
{
	if (speed == 500)
	{
		addChild(sprite = new Sprite("image/rock.png"));
		rect = sprite->rect;
	}
	else
	{
		addChild(sprite = new Sprite("image/arrows.png"));
		rect = sprite->rect;
	}

	rotationCenter = sprite->rect.center();
	rotation = dir;
}

Arrow::~Arrow()
{
}

void Arrow::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;
	Entity::update(dt);

	for_iter(iter, scene->objList)
	{
		if (((*iter)->isUnit && (*iter)->type != PLAYER) || (*iter)->type == CAVE)
		{
			if (circleCollision(center(), (*iter)->GetCollisionPos(), (*iter)->colRange + 7))
			{
				if ((*iter)->type == CAVE && speed == 800 && !((Cave*)*iter)->isBroken)
					(*iter)->hp = (*iter)->hp - 25 < 0 ? 0 : (*iter)->hp - 25;
				else if ((*iter)->type == CAVE && speed == 500)
					(*iter)->hp = (*iter)->hp - 500 < 0 ? 0 : (*iter)->hp - 500;
				else if (!((*iter)->type == CAVE) && speed != 500)
					((Unit*)*iter)->Attacked(angle(center(), (*iter)->GetCollisionPos()), speed / 10);
				scene->arrowList.remove(this);
				removeChild(this);
				return;
			}
		}
	}

	pos += Vec2(cos(dir), sin(dir)) * speed * dt;
	timer -= dt;

	if (timer <= 0)
	{
		sprite->color.a -= dt * 5;

		if (sprite->color.a <= 0)
		{
			scene->arrowList.remove(this);
			removeChild(this);
		}
	}
}