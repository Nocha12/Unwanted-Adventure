#include "DXUT.h"
#include "EndScene.h"
#include "World.h"
#include "MainScene.h"

EndScene::EndScene()
{
	addChild(ed = new Animation("image/ed", 12, 10, false, true));
	ed->stop = true;
}

EndScene::~EndScene()
{
}

void EndScene::update(float dt)
{
	Scene::update(dt);

	if (ed->stop && world.getKeyState(VK_LBUTTON) == 1)
	{
		ed->stop = false;
	}
	if (ed->currentFrame >= ed->textures.size() - 1)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			world.changeScene(new MainScene());
			return;
		}
	}
}