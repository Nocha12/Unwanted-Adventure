#include "DXUT.h"
#include "Tool.h"
#include "GameScene.h"
#include "World.h"

Tool::Tool(ToolType type) : type(type), speed(10)
{
	if (type == AX_ITEM)
		addChild(animation = new Animation("image/map/drop/ax", 8, 8, true, true));
	else if (type == BUCKET_ITEM)
		addChild(animation = new Animation("image/map/drop/bucket", 8, 8, true, true));
	else if (type == HAMMER_ITEM)
		addChild(animation = new Animation("image/map/drop/hammer", 8, 8, true, true));
	else if (type == PICK_ITEM)
		addChild(animation = new Animation("image/map/drop/pick", 8, 8, true, true));
	else if (type == SAW_ITEM)
		addChild(animation = new Animation("image/map/drop/saw", 8, 8, true, true));

	rect = animation->rect;
}

Tool::~Tool()
{
}

void Tool::update(float dt)
{
	Entity::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;

	if (pita(scene->player->GetCollisionPos(), center()) < 120)
	{
		scene->GetTile(center())->toolTile = false;
		moveRotation = angle(center(), scene->player->GetCollisionPos());
		pos += Vec2(cos(moveRotation), sin(moveRotation)) * dt * (speed += dt * 500);
	}

	if (pita(scene->player->GetCollisionPos(), center()) < 10)
	{
		if (type == AX_ITEM)
			scene->ui->toolSprite[AX_ITEM]->visible = false;
		else if (type == BUCKET_ITEM)
			scene->ui->toolSprite[BUCKET_ITEM]->visible = false;
		else if (type == HAMMER_ITEM)
			scene->ui->toolSprite[HAMMER_ITEM]->visible = false;
		else if (type == PICK_ITEM)
			scene->ui->toolSprite[PICK_ITEM]->visible = false;
		else if (type == SAW_ITEM)
			scene->ui->toolSprite[SAW_ITEM]->visible = false;

		scene->toolList.remove(this); 
		removeChild(this);
	}
}
