#include "DXUT.h"
#include "GameScene.h"
#include "World.h"
#include "GameManager.h"
#include "Player.h"
#include "Enemy.h"
#include "MainScene.h"
#include "EndScene.h"

GameScene::GameScene() : buildingFactory(this), waitBuild(nullptr), isInit(false), second(0), timer(0), minute(10), isStage2(false)
{
	ui = new UI(this);

	for (int i = 0; i < TILECOUNT; i++){
		for (int j = 0; j < TILECOUNT; j++){
			mapTile[i][j] = nullptr;
		}
	}
	stageMap = new StageMap(this);
	addChild(stageMap);

	player = new Player();
	player->SetCollisionPos(GetRandomSideTile(SAND_TILE)->GetCenter());
	addChild(player);

	follow(player);
	pos = -player->center() + offset;
	player->movePos = player->GetCollisionPos();
	objList.push_back(player);

	if (-pos.x < 0) pos.x = 0;
	if (-pos.y < 20) pos.y = -20;
	if (-pos.x > MAPWIDTH - 1280 - TILEWIDTH / 2) pos.x = -(MAPWIDTH - 1280 - TILEWIDTH / 2);
	if (-pos.y > MAPHEIGHT / 2 - 720 - TILEHEIGHT / 2) pos.y = -(MAPHEIGHT / 2 - 720 - TILEHEIGHT / 2);

	addChild(cursor = new Cursor());
	addChild(cursorImg[ATTACK_CURSOR] = new Sprite("image/ui/mouse/attack.png"));
	addChild(cursorImg[HAMMER_CURSOR] = new Sprite("image/ui/mouse/hammer.png"));
	addChild(cursorImg[PICK_CURSOR] = new Sprite("image/ui/mouse/rock.png"));
	addChild(cursorImg[SLEEP_CURSOR] = new Sprite("image/ui/mouse/sleep.png"));
	addChild(cursorImg[AX_CURSOR] = new Sprite("image/ui/mouse/tree.png"));
	addChild(cursorImg[X_CURSOR] = new Sprite("image/ui/mouse/x.png"));
	addChild(cursorImg[MOVE_CURSOR] = new Sprite("image/ui/mouse/main.png"));
	ChangeCursor(MOVE_CURSOR);
}

GameScene::~GameScene()
{
	SAFE_DELETE(ui);
}

void GameScene::render()
{
	Scene::render();

	for_iter(child, stageMap->children)
	{
		bool flag = true;
		if (-pos.x - 300 > (*child)->pos.x) flag = false;
		if (-pos.y - 300 > (*child)->pos.y) flag = false;
		if (-pos.x + 1280 < (*child)->pos.x) flag = false;
		if (-pos.y + 720  < (*child)->pos.y) flag = false;

		if (flag)
			(*child)->render();
	}

	for_iter(iter, arrowList)
		(*iter)->render();

	for_iter(iter, toolList)
		(*iter)->render();

	objList.sort([](Object *o1, Object *o2){
		return o1->GetCollisionPos().y < o2->GetCollisionPos().y ? true : false;
	});
	for_iter(iter, objList)
		(*iter)->render();

	if (waitBuild)
		waitBuild->render();

	for_iter(iter, dustList)
		(*iter)->render();
	for_iter(iter, particleList)
		(*iter)->render();

	ui->render();

	for_iter(iter, itemList)
		(*iter)->render();

	for_iter(iter, cursorImg)
		(*iter).second->render();
}

void GameScene::update(float dt)
{
	Scene::update(dt);
	ui->update(dt);
	
	for_iter(iter, cursorImg)
		(*iter).second->setCenter(world.getMousePos() - pos);

	if (player->ending)
	{
		gameManager.timeM2 = minute;
		gameManager.timeS2 = second;
		world.changeScene(new EndScene());
		return;
	}
	if (ui->gameOver->color.a >= 1 && world.getKeyState(VK_LBUTTON) == 1)
	{
		world.changeScene(new MainScene());
		return;
	}
	if (!isInit)
	{
		Tool *hammer = new Tool(HAMMER_ITEM);
		addChild(hammer);
		toolList.push_back(hammer);
		hammer->setCenter(GetRandomSideTile(SAND_TILE)->GetCenter());
		GetTile(hammer->center())->toolTile = true;
		Tool *ax = new Tool(AX_ITEM);
		addChild(ax);
		toolList.push_back(ax);
		ax->setCenter(GetRandomSideTile(SAND_TILE)->GetCenter());
		GetTile(ax->center())->toolTile = true;
		Tool *pick = new Tool(PICK_ITEM);
		addChild(pick);
		toolList.push_back(pick);
		pick->setCenter(GetRandomSideTile(SAND_TILE)->GetCenter());
		GetTile(pick->center())->toolTile = true;
		Tool *saw = new Tool(SAW_ITEM);
		addChild(saw);
		toolList.push_back(saw);
		saw->setCenter(GetRandomSideTile(SAND_TILE)->GetCenter());
		GetTile(saw->center())->toolTile = true;
		Tool *bucket = new Tool(BUCKET_ITEM);
		addChild(bucket);
		toolList.push_back(bucket);
		bucket->setCenter(GetRandomSideTile(SAND_TILE)->GetCenter());
		GetTile(bucket->center())->toolTile = true;

		for (int i = 0; i < 24; i++)
			addChild(buildingFactory.MakeBuilding(ROCK, true, false, GROUND_TILE));
		for (int i = 0; i < 3; i++)
			addChild(buildingFactory.MakeBuilding(ROCK, true, false, SAND_TILE));
		for (int i = 0; i < 30; i++)
			addChild(buildingFactory.MakeBuilding(TREE, true, false, GROUND_TILE));

		for (int i = 0; i < 20; i++)
		{
			Deer *deer = new Deer();
			deer->SetCollisionPos(GetRandomTile(GROUND_TILE)->GetCenter());
			addChild(deer);
			objList.push_back(deer);
		}
		isInit = true;
		ui->stage1Ex->color.a = 1;
	}
	if (world.getKeyState(VK_F9) == 1)
	{
		ui->toolSprite[AX_ITEM]->visible = false;
	}
	if (world.getKeyState(VK_F1) == 1)
	{
		for (int i = 0; i < 7; i++)
			ui->toolSprite[i]->visible = false;
		ui->arrowSprite->visible = true;
	}
	if (world.getKeyState(VK_F2) == 1)
	{
		for (int i = 0; i < 15; i++)
		{
			Item *item1 = new Item(ROCK_ITEM);
			ui->makeItemList.push_back(item1);
		}
		for (int i = 0; i < 15; i++)
		{
			Item *item2 = new Item(LOG_ITEM);
			ui->makeItemList.push_back(item2);
		}for (int i = 0; i < 15; i++)
		{
			Item *item3 = new Item(LEAF_ITEM);
			ui->makeItemList.push_back(item3);
		}for (int i = 0; i < 15; i++)
		{
			Item *item4 = new Item(ROPE_ITEM);
			ui->makeItemList.push_back(item4);
		}for (int i = 0; i < 15; i++)
		{
			Item *item5 = new Item(BRANCH_ITEM);
			ui->makeItemList.push_back(item5);
		}for (int i = 0; i < 15; i++)
		{
			Item *item6 = new Item(SKIN_ITEM);
			ui->makeItemList.push_back(item6);
		}

		ui->MakeItem(player->GetCollisionPos());
	}
	if (world.getKeyState(VK_F3) == 1)
	{
		ui->buildHome = true; ui->buildWorkRoom = true; ui->buildWell = true;
		ui->toolSprite[BUCKET_ITEM]->visible = false; ui->toolSprite[HAMMER_ITEM]->visible = false; ui->toolSprite[AX_ITEM]->visible = false; ui->toolSprite[PICK_ITEM]->visible = false; ui->toolSprite[SAW_ITEM]->visible = false;
	}
	if (world.getKeyState(VK_F4) == 1)
	{
		for_iter(iter, objList)
		{
			if ((*iter)->type == BOAR)
				((Unit*)*iter)->Attacked(0, 0, 1000);
		}
	}
	if (world.getKeyState(VK_F5) == 1)
	{
		for_iter(iter, objList)
		{
			if ((*iter)->type == CAVE)
				(*iter)->hp = 0;
		}
	}
	if (world.getKeyState(VK_SPACE) == 1)
	{
		follow(player);
	}

	if (ui->buildHome && ui->buildWorkRoom && ui->buildWell &&
		!ui->toolSprite[BUCKET_ITEM]->visible && !ui->toolSprite[HAMMER_ITEM]->visible && !ui->toolSprite[AX_ITEM]->visible&&!ui->toolSprite[PICK_ITEM]->visible && !ui->toolSprite[SAW_ITEM]->visible && !isStage2)
	{
		isStage2 = true;
		ui->hpSprite->visible = true;
		addChild(raft = buildingFactory.MakeBuilding(RAFT, true, true, SAND_TILE));
		
		for (int i = 0; i < 3; i++)
		{
			Building *c = buildingFactory.MakeBuilding(CAVE, true, true, GROUND_TILE, true);
			addChild(c);
			caveList.push_back((Cave*)c);
		}
		ui->stage2Ex->color.a = 1;
		gameManager.timeM1 = minute;
		gameManager.timeS1 = second;
		minute = 10;
		second = 0;
		player->food = 100;
		player->water = 100;
		player->sleep = 100;
	}

	if (world.getMousePos().x <= 10) pos.x += 800 * dt;
	if (world.getMousePos().y <= 10) pos.y += 800 * dt;
	if (world.getMousePos().x >= 1270) pos.x -= 800 * dt;
	if (world.getMousePos().y >= 710) pos.y -= 800 * dt;

	if (-pos.x < 0) pos.x = 0;
	if (-pos.y < 20) pos.y = -20;
	if (-pos.x > MAPWIDTH - 1280 - TILEWIDTH / 2) pos.x = -(MAPWIDTH - 1280 - TILEWIDTH / 2);
	if (-pos.y > MAPHEIGHT / 2 - 720 - TILEHEIGHT / 2) pos.y = -(MAPHEIGHT / 2 - 720 - TILEHEIGHT / 2);

	for_iter(dust, dustList)
	{
		(*dust)->scale += Vec2(dt, dt);
		(*dust)->color.a += dt;
		(*dust)->rotation += dt * 3;

		if ((*dust)->color.a >= 1)
		{
			removeChild(*dust);
			dust = dustList.erase(dust);
			if (dust == dustList.end())
				break;
		}
	}

	timer += dt;

	if (timer >= 1)
	{
		timer = 0;
		second--;
		if (second < 0 && minute >= 1)
		{
			second = 59;
			minute--;
		}
		else if (second < 0)
			second = 0;
	}
	if (minute == 0 && second == 0)
	{
		player->canMove = false;
		player->isDead = true;
		player->ChangeAnimation(DEAD);
		ui->gameOver->visible = true;
	}

	int minuteT = minute;
	int secondT = second;
	for (int i = 0; i < 4; ++i)
	{
		if (i < 2)
		{
			for (int j = 0; j < 10; ++j)
				if (ui->timerText[1 - i][j]->visible)
					ui->timerText[1 - i][j]->visible = false;
			ui->timerText[1 - i][minuteT % 10]->visible = true;
			minuteT /= 10;
		}
		else
		{
			for (int j = 0; j < 10; ++j)
				if (ui->timerText[5 - i][j]->visible)
					ui->timerText[5 - i][j]->visible = false;
			ui->timerText[5 - i][secondT % 10]->visible = true;
			secondT /= 10;
		}
	}

	if (isStage2)
	{
		int hpT = player->hp;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 10; ++j)
				if (ui->hpText[2 - i][j]->visible)
					ui->hpText[2 - i][j]->visible = false;

			ui->hpText[2 - i][hpT % 10]->visible = true;
			hpT /= 10;
		}
	}

	for_iter(iter, textEffectList)
	{
		(*iter)->pos.y -= dt * 100;
		(*iter)->color.a -= dt;
		if ((*iter)->color.a <= 0)
		{
			removeChild(*iter);
			iter = textEffectList.erase(iter);
			if (iter == textEffectList.end()) break;
		}
	}
} 

Tile* GameScene::GetTile(Vec2 pos)
{
	Vec2 p = Vec2((int)(pos.x + TILEWIDTH / 2) / TILEWIDTH, (int)(pos.y + TILEHEIGHT / 2) / TILEHEIGHT * 2);
	Vec2 t = Vec2((int)(pos.x + TILEWIDTH / 2) % TILEWIDTH, (int)(pos.y + TILEHEIGHT / 2) % TILEHEIGHT);

	float r = (TILEHEIGHT * 0.25f) / (TILEWIDTH * 0.25f);
	if (t.y < -r * t.x + TILEHEIGHT * 0.5f){
		p.x -= 1;
		p.y -= 1;
	}
	else if (t.y < r * t.x - TILEHEIGHT * 0.5f){
		p.y -= 1;
	}
	else if (t.y > r * t.x + TILEHEIGHT * 0.5f){
		p.x -= 1;
		p.y += 1;
	}
	else if (t.y > -r * t.x + TILEHEIGHT * 1.5f){
		p.y += 1;
	}

	p.x = min(max(p.x, 0), TILECOUNT - 1);
	p.y = min(max(p.y, 0), TILECOUNT - 1);
	return mapTile[int(p.x)][int(p.y)];
}

Tile* GameScene::GetRandomTile(TILE_CODE tileCode)
{
	vector<Tile*> tiles;
	for (int i = 0; i < TILECOUNT; i++){
		for (int j = 0; j < TILECOUNT; j++){
			if (mapTile[i][j]->code == tileCode && !mapTile[i][j]->toolTile){
				tiles.push_back(mapTile[i][j]);
			}
		}
	}
	return tiles[random(0, tiles.size() - 1)];
}

Tile* GameScene::GetRandomSideTile(TILE_CODE tileCode)
{
	vector<Tile*> tiles;
	for (int i = 0; i < TILECOUNT; i++){
		for (int j = 0; j < TILECOUNT; j++){
			if (mapTile[i][j]->code == tileCode && (i < 5 || i > TILECOUNT - 5 || j < 5 || j > TILECOUNT - 5) && !mapTile[i][j]->toolTile){
				tiles.push_back(mapTile[i][j]);
			}
		}
	}
	return tiles[random(0, tiles.size() - 1)];
}

void GameScene::ChangeCursor(CursorType type)
{
	for_iter(iter, cursorImg)
		(*iter).second->visible = false;
	cursorImg[type]->visible = true;
}