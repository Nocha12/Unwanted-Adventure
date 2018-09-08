#include "DXUT.h"
#include "UI.h"
#include "GameScene.h"
#include "GameManager.h"
#include "World.h"

UI::UI(Scene* scene) : gscene(scene), buildHome(false), buildWorkRoom(false), buildForge(false), buildWell(false)
{
	addChild(hpSprite = new Sprite("image/ui/hp.png"));
	hpSprite->visible = false;
	hpSprite->pos = Vec2(1202, 201);
	addChild(arrowSprite = new Sprite("image/ui/arrow.png"));
	arrowSprite->pos = Vec2(61, 466);
	arrowSprite->visible = false;
	addChild(blackOut = new Sprite("image/ui/blackOut.png"));
	blackOut->color.a = 0;
	addChild(redEffect = new Sprite("image/ui/red.png"));
	redEffect->color.a = 0;
	addChild(new Sprite("image/ui/1stage.png"));
	addChild(makeBuildingPage = new Sprite("image/ui/making.png"));
	makeBuildingPage->visible = false;
	addChild(makeWeaponPage = new Sprite("image/ui/making2.png"));
	makeWeaponPage->visible = false;
	addChild(book = new Sprite("image/ui/book.png"));
	book->scaleCenter = book->rect.center();
	book->pos = Vec2(1195, 301);

	addChild(homeBuildBtn = new Sprite("image/ui/makingButton.png"));
	addChild(wellBuildBtn = new Sprite("image/ui/makingButton.png"));
	addChild(workroomBuildBtn = new Sprite("image/ui/makingButton.png"));
	addChild(forgeBuildBtn = new Sprite("image/ui/makingButton.png"));
	addChild(backBtn = new Sprite("image/ui/xButton.png"));
	
	homeBuildBtn->scaleCenter = homeBuildBtn->rect.center();
	wellBuildBtn->scaleCenter = wellBuildBtn->rect.center();
	workroomBuildBtn->scaleCenter = workroomBuildBtn->rect.center();
	forgeBuildBtn->scaleCenter = forgeBuildBtn->rect.center();
	backBtn->scaleCenter = backBtn->rect.center();

	homeBuildBtn->pos = Vec2(572, 322);
	wellBuildBtn->pos = Vec2(572, 563);
	workroomBuildBtn->pos = Vec2(806, 322);
	forgeBuildBtn->pos = Vec2(806, 563);
	backBtn->pos = Vec2(886, 121);

	homeBuildBtn->visible = false;
	wellBuildBtn->visible = false;
	workroomBuildBtn->visible = false;
	forgeBuildBtn->visible = false;
	backBtn->visible = false;

	for (int q = 0; q < 10; q++)
	{
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				addChild(itemCountText[q][i][j] = new Sprite("image/ui/itemNum/" + to_string(j) + ".png"));
				itemCountText[q][i][j]->pos = Vec2(348 + 20 * i + 61 * q, 648);
				itemCountText[q][i][j]->visible = false;
			}
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(sleepText[i][j] = new Sprite("image/ui/itemNum/" + to_string(j) + ".png"));
			sleepText[i][j]->pos = Vec2(1108 + 20 * i, 240);
			sleepText[i][j]->visible = false;
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(foodText[i][j] = new Sprite("image/ui/itemNum/" + to_string(j) + ".png"));
			foodText[i][j]->pos = Vec2(1038 + 20 * i, 187);
			foodText[i][j]->visible = false;
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(waterText[i][j] = new Sprite("image/ui/itemNum/" + to_string(j) + ".png"));
			waterText[i][j]->pos = Vec2(1022 + 20 * i, 98);
			waterText[i][j]->visible = false;
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(hpText[i][j] = new Sprite("image/ui/itemNum/" + to_string(j) + ".png"));
			hpText[i][j]->pos = Vec2(1200 + 20 * i, 240);
			hpText[i][j]->visible = false;
		}
	}

	for (int i = 0; i < 7; i++)
	{
		addChild(toolSprite[i] = new Sprite("image/ui/black.png"));
		toolSprite[i]->pos = Vec2(4, 228 + 47.5f * i);
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(timerText[i][j] = new Sprite("image/ui/timeNum/" + to_string(j) + ".png"));
			if (i == 0)
				timerText[i][j]->pos = Vec2(1131, 97);
			else if (i == 1)
				timerText[i][j]->pos = Vec2(1154, 97);
			else if (i == 2)
				timerText[i][j]->pos = Vec2(1191, 97);
			else
				timerText[i][j]->pos = Vec2(1214, 97);
			timerText[i][j]->visible = false;
		}
		timerText[i][0]->visible = true;
	}
	addChild(stage1Ex = new Sprite("image/ui/text/1.png"));
	stage1Ex->pos = Vec2(357, 114);
	stage1Ex->color.a = 0;
	addChild(stage2Ex = new Sprite("image/ui/text/2.png"));
	stage2Ex->pos = Vec2(357, 114);
	stage2Ex->color.a = 0;

	addChild(gameOver = new Sprite("image/ui/text/over.png"));
	gameOver->color.a = 0;
	gameOver->visible = false;;
}

UI::~UI()
{
}

bool UI::UseItem(ItemType type, bool use, int useCount)
{
	for (int i = 0; i < itemCount.size(); i++)
	{
		if (itemType[i] == type && itemCount[i] >= useCount)
		{
			if (use)
			{
				itemCount[i] -= useCount;
				if (itemCount[i] == 0)
				{
					removeChild(itemSprite[i]);
					itemType[i] = DEFAULT_ITEM;
					
					for (int j = 0; j < 2; ++j)
					for (int k = 0; k < 10; ++k)
						itemCountText[i][j][k]->visible = false;
				}
			}
			return true;
		}
	}

	return false;
}

void UI::MakeItem(Vec2 pos)
{
	GameScene *scene = (GameScene*)world.currentScene;

	if (makeItemTimer.active)
		return;

	makeItemTimer.reset(0.6f / makeItemList.size(), makeItemList.size());
	makeItemTimer.onTick = [=](){
		scene->itemList.push_back(makeItemList[makeItemTimer.count.first]);
		scene->addChild(makeItemList[makeItemTimer.count.first]);
		makeItemList[makeItemTimer.count.first]->setCenter(pos);
	};
	makeItemTimer.onFinished = [=](){ makeItemList.clear(); };
}

void UI::PushItem(ItemType type)
{
	for (int i = 0; i < itemType.size(); i++)
	{
		if (itemType[i] == type)
		{
			itemCount[i] += 1;
			itemSprite[i]->visible = true;
		}
	}
}

void UI::ViewWeaponPage()
{
	if (!makeWeaponPage->visible)
	{	
		makeWeaponPage->visible = true;
		homeBuildBtn->visible = true;
		wellBuildBtn->visible = true;
		workroomBuildBtn->visible = true;
		forgeBuildBtn->visible = true;
		backBtn->visible = true;

		if (UseItem(BRANCH_ITEM, false, 5) && UseItem(ROCK_ITEM, false, 5) && !arrowSprite->visible)
		{
			homeBuildBtn->color.r = 1;
			homeBuildBtn->color.g = 1;
			homeBuildBtn->color.b = 1;
		}
		else
		{
			homeBuildBtn->color.r = 0.5f;
			homeBuildBtn->color.g = 0.5f;
			homeBuildBtn->color.b = 0.5f;
		}

		if (UseItem(LOG_ITEM, false, 5) && UseItem(ROPE_ITEM, false, 5) && UseItem(ROCK_ITEM, false, 10))
		{
			wellBuildBtn->color.r = 1;
			wellBuildBtn->color.g = 1;
			wellBuildBtn->color.b = 1;
		}
		else
		{
			wellBuildBtn->color.r = 0.5f;
			wellBuildBtn->color.g = 0.5f;
			wellBuildBtn->color.b = 0.5f;
		}

		if (UseItem(ROPE_ITEM, false, 1) && UseItem(BRANCH_ITEM, false, 5) && toolSprite[BOW_ITEM]->visible)
		{
			workroomBuildBtn->color.r = 1;
			workroomBuildBtn->color.g = 1;
			workroomBuildBtn->color.b = 1;
		}
		else
		{
			workroomBuildBtn->color.r = 0.5f;
			workroomBuildBtn->color.g = 0.5f;
			workroomBuildBtn->color.b = 0.5f;
		}

		if (UseItem(BRANCH_ITEM, false, 5) && !toolSprite[BOW_ITEM]->visible && toolSprite[CROSSBOW_ITEM]->visible)
		{
			forgeBuildBtn->color.r = 1;
			forgeBuildBtn->color.g = 1;
			forgeBuildBtn->color.b = 1;
		}
		else
		{
			forgeBuildBtn->color.r = 0.5f;
			forgeBuildBtn->color.g = 0.5f;
			forgeBuildBtn->color.b = 0.5f;
		}
	}
}

Vec2 UI::GetItemPos(ItemType type)
{
	bool isPushed = false;
	for (int i = 0; i < itemType.size(); i++)
	{
		if (itemType[i] == type)
		{
			return itemSprite[i]->pos;
		}
	}
	if (!isPushed)
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

		for (int i = 0; i < itemSprite.size(); i++)
		if (itemType[i] == DEFAULT_ITEM)
		{
			spr->pos = Vec2(342 + i * 61, 661);
			itemSprite[i] = spr;
			spr->scaleCenter = spr->rect.center();
			spr->visible = false;
			itemType[i] = type;
			return spr->pos;
		}
	
		spr->pos = Vec2(342 + itemSprite.size() * 61, 661);
		itemSprite.push_back(spr);
		spr->scaleCenter = spr->rect.center();
		spr->visible = false;
		itemCount.push_back(0);
		itemType.push_back(type);
		return spr->pos;
	}
}


void UI::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;
	Entity::update(dt);

	for (int i = 0; i < itemCount.size(); i++)
	{
		if (itemType[i] == WATERBUCKET_ITEM || itemType[i] == ORANGE_ITEM || itemType[i] == MEAT_ITEM)
		{
			if (itemSprite[i]->rectWithPos().intersects(world.getMousePos()))
			{
				itemSprite[i]->scale = Vec2(1.2f, 1.2f);
				if (world.getKeyState(VK_LBUTTON) == 1 && scene->player->canMove)
				{
					Sprite *textE = nullptr;
					if (itemType[i] == WATERBUCKET_ITEM)
					{
						scene->player->water = scene->player->water + 20 > 100 ? 100 : scene->player->water + 20;
						scene->player->addChild(textE = new Sprite("image/ui/water20.png"));
						UseItem(WATERBUCKET_ITEM, true, 1);
					}
					if (itemType[i] == ORANGE_ITEM)
					{
						scene->player->food = scene->player->food + 10 > 100 ? 100 : scene->player->food + 10;
						scene->player->addChild(textE = new Sprite("image/ui/food10.png"));
						UseItem(ORANGE_ITEM, true, 1);
					}
					if (itemType[i] == MEAT_ITEM)
					{
						scene->player->food = scene->player->food + 20 > 100 ? 100 : scene->player->food + 20;
						scene->player->addChild(textE = new Sprite("image/ui/food20.png"));
						UseItem(MEAT_ITEM, true, 1);
					}
					scene->textEffectList.push_back(textE);
					textE->setCenter(scene->player->rect.center());
					return;
				}
			}
			else itemSprite[i]->scale = Vec2(1, 1);
		}
	}

	for (int q = 0; q < itemSprite.size(); q++)
	{
		if (itemType[q] == DEFAULT_ITEM)
			continue;

		int itemCountT = itemCount[q];
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 10; ++j)
			if (itemCountText[q][1 - i][j]->visible)
				itemCountText[q][1 - i][j]->visible = false;

			itemCountText[q][1 - i][itemCountT % 10]->visible = true;
			itemCountT /= 10;
		}
	}

	if (book->rectWithPos().intersects(world.getMousePos()) && !makeBuildingPage->visible)
	{
		book->scale = Vec2(1.2f, 1.2f);
		if (world.getKeyState(VK_LBUTTON) == 1 && scene->player->canMove)
		{
			makeBuildingPage->visible = true;
			homeBuildBtn->visible = true;
			wellBuildBtn->visible = true;
			workroomBuildBtn->visible = true;
			forgeBuildBtn->visible = true;
			backBtn->visible = true;

			if (UseItem(LOG_ITEM, false, 8) && UseItem(BRANCH_ITEM, false, 3) && UseItem(LEAF_ITEM, false, 4) && !toolSprite[HAMMER_ITEM]->visible && !toolSprite[SAW_ITEM]->visible)
			{
				homeBuildBtn->color.r = 1;
				homeBuildBtn->color.g = 1;
				homeBuildBtn->color.b = 1;
			}
			else
			{
				homeBuildBtn->color.r = 0.5f;
				homeBuildBtn->color.g = 0.5f;
				homeBuildBtn->color.b = 0.5f;
			}

			if (UseItem(ROCK_ITEM, false, 5) && UseItem(BRANCH_ITEM, false, 3) && UseItem(SKIN_ITEM, false, 3) && UseItem(ROPE_ITEM, false, 2) && !toolSprite[HAMMER_ITEM]->visible && !toolSprite[BUCKET_ITEM]->visible)
			{
				wellBuildBtn->color.r = 1;
				wellBuildBtn->color.g = 1;
				wellBuildBtn->color.b = 1;
			}
			else
			{
				wellBuildBtn->color.r = 0.5f;
				wellBuildBtn->color.g = 0.5f;
				wellBuildBtn->color.b = 0.5f;
			}

			if (UseItem(LOG_ITEM, false, 3) && UseItem(BRANCH_ITEM, false, 2) && !toolSprite[HAMMER_ITEM]->visible && !toolSprite[SAW_ITEM]->visible)
			{
				workroomBuildBtn->color.r = 1;
				workroomBuildBtn->color.g = 1;
				workroomBuildBtn->color.b = 1;
			}
			else
			{
				workroomBuildBtn->color.r = 0.5f;
				workroomBuildBtn->color.g = 0.5f;
				workroomBuildBtn->color.b = 0.5f;
			}

			if (UseItem(LOG_ITEM, false, 6) && UseItem(BRANCH_ITEM, false, 5) && UseItem(ROCK_ITEM, false, 10) && UseItem(SKIN_ITEM, false, 4) && !toolSprite[HAMMER_ITEM]->visible && !toolSprite[SAW_ITEM]->visible)
			{
				forgeBuildBtn->color.r = 1;
				forgeBuildBtn->color.g = 1;
				forgeBuildBtn->color.b = 1;
			}
			else
			{
				forgeBuildBtn->color.r = 0.5f;
				forgeBuildBtn->color.g = 0.5f;
				forgeBuildBtn->color.b = 0.5f;
			}

			return;
		}
	}
	else
		book->scale = Vec2(1, 1);

	if (makeBuildingPage->visible)
	{
		if (homeBuildBtn->rectWithPos().intersects(world.getMousePos()) && homeBuildBtn->color.r == 1)
		{
			homeBuildBtn->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				makeBuildingPage->visible = false;
				homeBuildBtn->visible = false;
				wellBuildBtn->visible = false;
				workroomBuildBtn->visible = false;
				forgeBuildBtn->visible = false;
				backBtn->visible = false;
				UseItem(LOG_ITEM, true, 8); UseItem(BRANCH_ITEM, true, 3); UseItem(LEAF_ITEM, true, 4);
				scene->addChild(scene->buildingFactory.MakeBuilding(HOME));
				return;
			}
		}
		else
			homeBuildBtn->scale = Vec2(1, 1);

		if (wellBuildBtn->rectWithPos().intersects(world.getMousePos()) && wellBuildBtn->color.r == 1)
		{
			wellBuildBtn->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				makeBuildingPage->visible = false;
				homeBuildBtn->visible = false;
				wellBuildBtn->visible = false;
				workroomBuildBtn->visible = false;
				forgeBuildBtn->visible = false;
				backBtn->visible = false;
				UseItem(ROCK_ITEM, true, 5); UseItem(BRANCH_ITEM, true, 3); UseItem(SKIN_ITEM, true, 3); UseItem(ROPE_ITEM, true, 2);
				scene->addChild(scene->buildingFactory.MakeBuilding(WELL));
				return;
			}
		}
		else
			wellBuildBtn->scale = Vec2(1, 1);

		if (workroomBuildBtn->rectWithPos().intersects(world.getMousePos()) && workroomBuildBtn->color.r == 1)
		{
			workroomBuildBtn->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				makeBuildingPage->visible = false;
				homeBuildBtn->visible = false;
				wellBuildBtn->visible = false;
				workroomBuildBtn->visible = false;
				forgeBuildBtn->visible = false;
				backBtn->visible = false;
				UseItem(LOG_ITEM, true, 5); UseItem(BRANCH_ITEM, true, 2);
				scene->addChild(scene->buildingFactory.MakeBuilding(WORKROOM));
				return;
			}
		}
		else
			workroomBuildBtn->scale = Vec2(1, 1);

		if (forgeBuildBtn->rectWithPos().intersects(world.getMousePos()) && forgeBuildBtn->color.r == 1)
		{
			forgeBuildBtn->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				makeBuildingPage->visible = false;
				homeBuildBtn->visible = false;
				wellBuildBtn->visible = false;
				workroomBuildBtn->visible = false;
				forgeBuildBtn->visible = false;
				backBtn->visible = false;
				UseItem(LOG_ITEM, true, 6); UseItem(BRANCH_ITEM, true, 5); UseItem(ROCK_ITEM, true, 10); UseItem(SKIN_ITEM, true, 4);
				scene->addChild(scene->buildingFactory.MakeBuilding(FORGE));
				return;
			}
		}
		else
			forgeBuildBtn->scale = Vec2(1, 1);

		if (backBtn->rectWithPos().intersects(world.getMousePos()))
		{
			backBtn->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				makeBuildingPage->visible = false;
				homeBuildBtn->visible = false;
				wellBuildBtn->visible = false;
				workroomBuildBtn->visible = false;
				forgeBuildBtn->visible = false;
				backBtn->visible = false;
				return;
			}
		}
		else
			backBtn->scale = Vec2(1, 1);
	}

	if (makeWeaponPage->visible)
	{
		if (homeBuildBtn->rectWithPos().intersects(world.getMousePos()) && homeBuildBtn->color.r == 1)
		{
			homeBuildBtn->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				makeWeaponPage->visible = false;
				homeBuildBtn->visible = false;
				wellBuildBtn->visible = false;
				workroomBuildBtn->visible = false;
				forgeBuildBtn->visible = false;
				backBtn->visible = false;
				UseItem(BRANCH_ITEM, true, 5); UseItem(ROCK_ITEM, true, 5);
				arrowSprite->visible = true;
				return;
			}
		}
		else
			homeBuildBtn->scale = Vec2(1, 1);

		if (wellBuildBtn->rectWithPos().intersects(world.getMousePos()) && wellBuildBtn->color.r == 1)
		{
			wellBuildBtn->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				makeWeaponPage->visible = false;
				homeBuildBtn->visible = false;
				wellBuildBtn->visible = false;
				workroomBuildBtn->visible = false;
				forgeBuildBtn->visible = false;
				backBtn->visible = false;
				UseItem(LOG_ITEM, true, 5); UseItem(ROPE_ITEM, true, 5); UseItem(ROCK_ITEM, true, 10);
				scene->addChild(scene->buildingFactory.MakeBuilding(CATAPULT));
				return;
			}
		}
		else
			wellBuildBtn->scale = Vec2(1, 1);

		if (workroomBuildBtn->rectWithPos().intersects(world.getMousePos()) && workroomBuildBtn->color.r == 1)
		{
			workroomBuildBtn->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				makeWeaponPage->visible = false;
				homeBuildBtn->visible = false;
				wellBuildBtn->visible = false;
				workroomBuildBtn->visible = false;
				forgeBuildBtn->visible = false;
				backBtn->visible = false;
				UseItem(ROPE_ITEM, true, 1); UseItem(BRANCH_ITEM, true, 5);
				toolSprite[BOW_ITEM]->visible = false;
				return;
			}
		}
		else
			workroomBuildBtn->scale = Vec2(1, 1);

		if (forgeBuildBtn->rectWithPos().intersects(world.getMousePos()) && forgeBuildBtn->color.r == 1)
		{
			forgeBuildBtn->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				makeWeaponPage->visible = false;
				homeBuildBtn->visible = false;
				wellBuildBtn->visible = false;
				workroomBuildBtn->visible = false;
				forgeBuildBtn->visible = false;
				backBtn->visible = false;
				UseItem(BRANCH_ITEM, true, 5);
				toolSprite[CROSSBOW_ITEM]->visible = false;
				return;
			}
		}
		else
			forgeBuildBtn->scale = Vec2(1, 1);

		if (backBtn->rectWithPos().intersects(world.getMousePos()))
		{
			backBtn->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				makeWeaponPage->visible = false;
				homeBuildBtn->visible = false;
				wellBuildBtn->visible = false;
				workroomBuildBtn->visible = false;
				forgeBuildBtn->visible = false;
				backBtn->visible = false;
				return;
			}
		}
		else
			backBtn->scale = Vec2(1, 1);
	}

	stage1Ex->color.a = stage1Ex->color.a - dt * 0.5f < 0 ? 0 : stage1Ex->color.a - dt * 0.5f;
	stage2Ex->color.a = stage2Ex->color.a - dt * 0.5f < 0 ? 0 : stage2Ex->color.a - dt * 0.5f;
	redEffect->color.a = redEffect->color.a - dt < 0 ? 0 : redEffect->color.a - dt;

	if (gameOver->visible)
		gameOver->color.a = gameOver->color.a + dt > 1 ? 1 : gameOver->color.a + dt;
}

void UI::render()
{
	arrowSprite->render();
	Entity::render();
	for (int q = 0; q < 10; q++)
	for (int i = 0; i < 2; ++i)
	for (int j = 0; j < 10; ++j)
		itemCountText[q][i][j]->render();
	makeBuildingPage->render();
	makeWeaponPage->render();

	homeBuildBtn->render(); 
	wellBuildBtn->render();
	workroomBuildBtn->render();
	forgeBuildBtn->render();
	backBtn->render();
	redEffect->render();
	blackOut->render();
	gameOver->render();
}
