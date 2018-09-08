#pragma once
#include "Scene.h"
#include "Animation.h"

class EndScene :
	public Scene
{
public:
	EndScene();
	~EndScene();
	
	Animation *ed;

	void update(float dt);
};

