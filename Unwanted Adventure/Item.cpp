#include "DXUT.h"
#include "Item.h"
#include "World.h"
#include "GameScene.h"

Item::Item(ItemType type) : type(type), speed(600), isSetGoalPos(false), isFollow(false)
{
	Sprite *spr = nullptr;
	if (type == SKIN_ITEM)
		addChild(spr = new Sprite("image/ui/icon/animalskin.png"));
	if (type == WATERBUCKET_ITEM)
		addChild(spr = new Sprite("image/ui/icon/bucket.png"));
	if (type == LOG_ITEM)
		addChild(spr = new Sprite("image/ui/icon/firewood.png"));
	if (type == LEAF_ITEM)
		addChild(spr = new Sprite("image/ui/icon/leaf.png"));
	if (type == ORANGE_ITEM)
		addChild(spr = new Sprite("image/ui/icon/orange.png"));
	if (type == ROCK_ITEM)
		addChild(spr = new Sprite("image/ui/icon/rock.png"));
	if (type == ROPE_ITEM)
		addChild(spr = new Sprite("image/ui/icon/rope.png"));
	if (type == BRANCH_ITEM)
		addChild(spr = new Sprite("image/ui/icon/stem.png"));
	if (type == MEAT_ITEM)
		addChild(spr = new Sprite("image/ui/icon/meat.png"));

	moveRotation = D3DXToRadian(random(0, 360));

	rect = spr->rect;
	speed += random(-100, 50);
}

Item::~Item()
{
}

void Item::update(float dt)
{
	if (removing)
		return;

	Entity::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;

	if (!isSetGoalPos)
	{
		isSetGoalPos = true;
		goalPos = scene->ui->GetItemPos(type);
	}

	pos += Vec2(cos(moveRotation), sin(moveRotation)) * dt * speed;
	
	if (speed <= 10 && !isFollow)
	{
		moveRotation = angle(pos, goalPos - scene->pos);
		speed = 1700;
		isFollow = true;
	}
	if (!isFollow)
		speed -= dt * speed * 8;
	else
	{
		moveRotation = angle(pos, goalPos - scene->pos);
		speed -= dt * speed;
	}

	if (pita(goalPos - scene->pos, pos) < 50)
	{
		scene->itemList.remove(this);
		removeChild(this);
		scene->ui->PushItem(type);
	}
}
