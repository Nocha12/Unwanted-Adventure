#pragma once
#include "Scene.h"
#include "Animation.h"

class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();

	Sprite *gameStartB;
	Sprite *exitB;
	Sprite *howToB;
	Sprite *introB;
	Sprite *resultB;
	Sprite *howToEx[4];
	Sprite *introEx;
	Sprite *resultEx;

	Sprite *cursor;

	Sprite *s1TimerText[4][10];
	Sprite *s2TimerText[4][10];

	int howToCount;

	void update(float dt);
};

