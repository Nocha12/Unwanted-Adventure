#include "DXUT.h"
#include "MainScene.h"
#include "World.h"
#include "GameScene.h"
#include "GameManager.h"

MainScene::MainScene() : howToCount(-1)
{
	Animation *ani = new Animation("image/title/ani", 8, 8, true, true);
	addChild(ani);

	Sprite *title = new Sprite("image/title/title.png");
	addChild(title);
	addChild(gameStartB = new Sprite("image/title/gameStart.png"));
	addChild(introB = new Sprite("image/title/introduceB.png"));
	addChild(howToB = new Sprite("image/title/howto.png"));
	addChild(resultB = new Sprite("image/title/resultB.png"));
	addChild(exitB = new Sprite("image/title/exit.png"));
	for (int i = 0; i < 4; i++)
	{
		addChild(howToEx[i] = new Sprite("image/title/howto/" + to_string(i) + ".png"));
		howToEx[i]->visible = false;
	}
	addChild(introEx = new Sprite("image/title/introduce.png"));
	introEx->visible = false;
	addChild(resultEx = new Sprite("image/title/result.png"));
	resultEx->visible = false;

	gameStartB->scaleCenter = gameStartB->rect.center();
	introB->scaleCenter = introB->rect.center();
	howToB->scaleCenter = howToB->rect.center();
	resultB->scaleCenter = resultB->rect.center();
	exitB->scaleCenter = exitB->rect.center();

	title->pos = Vec2(678, 58);
	gameStartB->pos = Vec2(942, 320);
	introB->pos = Vec2(976, 468);
	howToB->pos = Vec2(916, 392);
	exitB->pos = Vec2(960, 621);
	resultB->pos = Vec2(1042, 544);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(s1TimerText[i][j] = new Sprite("image/ui/timeNum/" + to_string(j) + ".png"));
			if (i == 0)
				s1TimerText[i][j]->pos = Vec2(588, 323);
			else if (i == 1)
				s1TimerText[i][j]->pos = Vec2(610, 323);
			else if (i == 2)
				s1TimerText[i][j]->pos = Vec2(659, 323);
			else
				s1TimerText[i][j]->pos = Vec2(681, 323);
			s1TimerText[i][j]->visible = false;
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(s2TimerText[i][j] = new Sprite("image/ui/timeNum/" + to_string(j) + ".png"));
			if (i == 0)
				s2TimerText[i][j]->pos = Vec2(588, 474);
			else if (i == 1)
				s2TimerText[i][j]->pos = Vec2(610, 474);
			else if (i == 2)
				s2TimerText[i][j]->pos = Vec2(659, 474);
			else
				s2TimerText[i][j]->pos = Vec2(681, 474);
			s2TimerText[i][j]->visible = false;
		}
	}

	addChild(cursor = new Sprite("image/ui/mouse/main.png"));
}

MainScene::~MainScene()
{
}

void MainScene::update(float dt)
{
	Scene::update(dt);
	cursor->setCenter(world.getMousePos());
	if (howToCount >= 0)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			if (howToCount < 3)
			{
				howToEx[howToCount]->visible = false;
				howToEx[++howToCount]->visible = true;
			}
			else
			{
				howToEx[3]->visible = false;
				howToCount = -1;
			}
			return;
		}
	}
	else if (introEx->visible)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			introEx->visible = false;
			return;
		}
	}
	else if (resultEx->visible)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			resultEx->visible = false;

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					s1TimerText[i][j]->visible = false;
					s2TimerText[i][j]->visible = false;
				}
			}

			return;
		}
	}
	else
	{
		if (gameStartB->rectWithPos().intersects(world.getMousePos()))
		{
			gameStartB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				gameManager.reset();
				world.changeScene(new GameScene());
				return;
			}
		}
		else
			gameStartB->scale = Vec2(1, 1);

		if (howToB->rectWithPos().intersects(world.getMousePos()))
		{
			howToB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				howToCount = 0;
				howToEx[howToCount]->visible = true;
				return;
			}
		}
		else
			howToB->scale = Vec2(1, 1);

		if (introB->rectWithPos().intersects(world.getMousePos()))
		{
			introB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				introEx->visible = true;
				return;
			}
		}
		else
			introB->scale = Vec2(1, 1);

		if (resultB->rectWithPos().intersects(world.getMousePos()))
		{
			resultB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				resultEx->visible = true;

				int minuteT = gameManager.timeM1;
				int secondT = gameManager.timeS1;
				for (int i = 0; i < 4; ++i)
				{
					if (i < 2)
					{
						for (int j = 0; j < 10; ++j)
						if (s1TimerText[1 - i][j]->visible)
							s1TimerText[1 - i][j]->visible = false;
						s1TimerText[1 - i][minuteT % 10]->visible = true;
						minuteT /= 10;
					}
					else
					{
						for (int j = 0; j < 10; ++j)
						if (s1TimerText[5 - i][j]->visible)
							s1TimerText[5 - i][j]->visible = false;
						s1TimerText[5 - i][secondT % 10]->visible = true;
						secondT /= 10;
					}
				}

				int minuteT2 = gameManager.timeM2;
				int secondT2 = gameManager.timeS2;
				for (int i = 0; i < 4; ++i)
				{
					if (i < 2)
					{
						for (int j = 0; j < 10; ++j)
						if (s2TimerText[1 - i][j]->visible)
							s2TimerText[1 - i][j]->visible = false;
						s2TimerText[1 - i][minuteT2 % 10]->visible = true;
						minuteT2 /= 10;
					}
					else
					{
						for (int j = 0; j < 10; ++j)
						if (s2TimerText[5 - i][j]->visible)
							s2TimerText[5 - i][j]->visible = false;
						s2TimerText[5 - i][secondT2 % 10]->visible = true;
						secondT2 /= 10;
					}
				}

				return;
			}
		}
		else
			resultB->scale = Vec2(1, 1);

		if (exitB->rectWithPos().intersects(world.getMousePos()))
		{
			exitB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				exit(1);
				return;
			}
		}
		else
			exitB->scale = Vec2(1, 1);
	}
}

