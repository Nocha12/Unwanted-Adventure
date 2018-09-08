#pragma once
#include "Building.h"
class Cave :
	public Building
{
public:
	Cave();
	~Cave();

	void update(float dt);

	void MakeEnemy(ObjectType type);
	Vec2 spawnPos;
	bool isBroken;
	float timer;
};

