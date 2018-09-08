#pragma once
#include "Scene.h"
#include "Tile.h"
#include "StageMap.h"
#include "UI.h"
#include "Player.h"
#include "Cursor.h"
#include "Building.h"
#include "BuildingFactory.h"
#include "Sprite.h"
#include "Particle.h"
#include "Cave.h"
#include "Arrow.h"
#include "Tool.h"
#include "Item.h"

class GameScene :
	public Scene
{
public:
	GameScene();
	~GameScene();

	void render();
	void update(float dt);

	UI* ui;

	StageMap* stageMap;
	Tile* mapTile[TILECOUNT][TILECOUNT];
	Tile* GetTile(Vec2 pos);
	Tile* GetRandomTile(TILE_CODE tileCode);
	Tile* GetRandomSideTile(TILE_CODE tileCode);
	list<Object*> objList;
	list<Sprite*> dustList;
	list<Particle*> particleList;
	list<Cave*> caveList;
	list<Arrow*> arrowList;
	list<Tool*> toolList;
	list<Item*> itemList;
	list<Sprite*> textEffectList;

	Cursor *cursor;
	Player *player;
	Building *raft;
	Building *waitBuild;
	BuildingFactory buildingFactory;

	bool isStage2;
	bool isInit;
	float timer;
	int second;
	int minute;

	map<CursorType, Sprite*> cursorImg;

	void ChangeCursor(CursorType type);
};

