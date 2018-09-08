#include "DXUT.h"
#include "Tree.h"
#include "World.h"
#include "GameScene.h"

Tree::Tree() : count(0)
{
	addChild(sprite = new Sprite("image/map/tree.png"));

	addChild(logBtn = new Sprite("image/ui/tree/firewood.png"));
	addChild(orangeBtn = new Sprite("image/ui/tree/fruit.png"));
	addChild(leafBtn = new Sprite("image/ui/tree/leaf.png"));
	addChild(branchBtn = new Sprite("image/ui/tree/trunk.png"));

	logBtn->visible = false;
	orangeBtn->visible = false;
	leafBtn->visible = false;
	branchBtn->visible = false;

	collisionPos = Vec2(186 + TILEWIDTH / 2, 251);

	branchBtn->pos = Vec2(71, 42) - collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2);
	logBtn->pos = Vec2(43, 165) - collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2);
	orangeBtn->pos = Vec2(251, 42) - collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2);
	leafBtn->pos = Vec2(278, 165) - collisionPos + Vec2(TILEWIDTH, TILEHEIGHT / 2);

	branchBtn->scaleCenter = branchBtn->rect.center();
	orangeBtn->scaleCenter = orangeBtn->rect.center();
	logBtn->scaleCenter = logBtn->rect.center();
	leafBtn->scaleCenter = leafBtn->rect.center();
}

Tree::~Tree()
{
}

void Tree::CutFinish()
{
	GameScene *scene = (GameScene*)world.currentScene;
	
	scene->ui->MakeItem(GetCollisionPos());
	
	scene->player->isCuttingTree = false;
	scene->player->canMove = true;
	scene->player->setTree = false;
	logBtn->visible = false;
	orangeBtn->visible = false;
	leafBtn->visible = false;
	branchBtn->visible = false;

	onDead();
}

void Tree::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;

	Vec2 mousePos = world.getMousePos() - scene->pos - pos;

	if (orangeBtn->rectWithPos().intersects(mousePos) && orangeBtn->visible)
	{
		scene->ChangeCursor(AX_CURSOR);
		orangeBtn->scale = Vec2(1.2f, 1.2f);
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			count++;
			for (int i = 0; i < random(1, 2); i++)
			{
				Item *item = new Item(ORANGE_ITEM);
				scene->ui->makeItemList.push_back(item);
			}

			orangeBtn->visible = false;
			return;
		}
	}
	else
		orangeBtn->scale = Vec2(1, 1);

	if (leafBtn->rectWithPos().intersects(mousePos) && leafBtn->visible)
	{
		scene->ChangeCursor(AX_CURSOR);
		leafBtn->scale = Vec2(1.2f, 1.2f);
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			for (int i = 0; i < random(2, 3); i++)
			{
				Item *item = new Item(LEAF_ITEM);
				scene->ui->makeItemList.push_back(item);
			}

			if (count != 1)
				CutFinish();

			leafBtn->visible = false;
			count++;
			return;
		}
	}
	else
		leafBtn->scale = Vec2(1, 1);

	if (branchBtn->rectWithPos().intersects(mousePos) && branchBtn->visible)
	{
		scene->ChangeCursor(AX_CURSOR);
		branchBtn->scale = Vec2(1.2f, 1.2f);
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			for (int i = 0; i < random(2, 3); i++)
			{
				Item *item = new Item(BRANCH_ITEM);
				scene->ui->makeItemList.push_back(item);
			}
			if (count != 2)
				CutFinish();

			branchBtn->visible = false;
			count++;
			return;
		}
	}
	else
		branchBtn->scale = Vec2(1, 1);

	if (logBtn->rectWithPos().intersects(mousePos) && logBtn->visible)
	{
		scene->ChangeCursor(AX_CURSOR);
		logBtn->scale = Vec2(1.2f, 1.2f);
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			for (int i = 0; i < random(2, 3); i++)
			{
				Item *item = new Item(LOG_ITEM);
				scene->ui->makeItemList.push_back(item);
			}

			logBtn->visible = false;
			count++;
			CutFinish();
			return;
		}
	}
	else
		logBtn->scale = Vec2(1, 1);
}
