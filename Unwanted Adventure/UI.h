#pragma once
#include "Entity.h"
#include "Scene.h"
#include "Sprite.h"
#include "Item.h"
#include "Timer.h"

class UI :
	public Entity
{
public:
	UI(Scene* scene);
	~UI();

	void update(float dt);
	void render();

	Scene* gscene;
	
	Sprite *book;
	Sprite *makeBuildingPage;
	Sprite *makeWeaponPage;
	Sprite *homeBuildBtn;
	Sprite *wellBuildBtn;
	Sprite *workroomBuildBtn;
	Sprite *forgeBuildBtn;
	Sprite *backBtn;
	Sprite *blackOut;
	Sprite *hpSprite;

	Sprite *mapUI[TILECOUNT][TILECOUNT];

	vector<Item*> makeItemList;
	vector<Sprite*> itemSprite;
	vector<ItemType> itemType;
	vector<int> itemCount;

	Sprite *sleepText[3][10];
	Sprite *foodText[3][10];
	Sprite *waterText[3][10];
	Sprite *hpText[3][10];

	Sprite *timerText[4][10];
	Sprite *itemCountText[10][2][10];
	Sprite *toolSprite[7];
	Sprite *arrowSprite;

	Sprite *stage1Ex;
	Sprite *stage2Ex;
	Sprite *redEffect;
	Sprite *gameOver;

	void ViewWeaponPage();
	void PushItem(ItemType type);
	Vec2 GetItemPos(ItemType type);
	void MakeItem(Vec2 pos);
	bool UseItem(ItemType type, bool use = false, int useCount = 0);

	bool buildHome;
	bool buildWorkRoom;
	bool buildForge;
	bool buildWell;

	Timer makeItemTimer;
};

