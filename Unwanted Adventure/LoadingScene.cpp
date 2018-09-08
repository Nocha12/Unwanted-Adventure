#include "DXUT.h"
#include "LoadingScene.h"
#include "Asset.h"
#include "MainScene.h"
#include "World.h"

LoadingScene::LoadingScene()
{
	Sprite *sprite = new Sprite("image/load/back.png");
	addChild(sprite);
	addChild(p = new Animation("image/load/ani", 12, 10, true, true));
	p->pos = Vec2(162, 446);
	addChild(bar = new Sprite("image/load/bar.png"));
	bar->pos = Vec2(225, 556);
	addChild(op = new Animation("image/op", 25, 8, true, true));
	op->stop = true;
	op->visible = false;

	asset.ready("image");
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::update(float dt)
{
	Scene::update(dt);

	if (op->visible)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			op->stop = false;
		}

		if (op->currentFrame >= op->textures.size() - 1)
		{
			world.changeScene(new MainScene());
			return;
		}

		return;
	}

	asset.loadNext();
	bar->visibleRect.right = bar->rect.right * asset.filesLoaded / asset.filesToLoad;
	p->pos.x = 331 + bar->visibleRect.right - 225;
	if (asset.filesLoaded == asset.filesToLoad)
	{
		op->visible = true;
		return;
	}
}


