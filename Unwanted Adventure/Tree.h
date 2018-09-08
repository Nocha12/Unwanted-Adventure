#pragma once
#include "Building.h"
#include "Sprite.h"

class Tree :
	public Building
{
public:
	Tree();
	~Tree();

	void update(float dt);

	Sprite *logBtn;
	Sprite *orangeBtn;
	Sprite *leafBtn;
	Sprite *branchBtn;

	void CutFinish();

	int count;
};

