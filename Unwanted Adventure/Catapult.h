#pragma once
#include "Building.h"
class Catapult : public Building
{
public:
	Catapult();
	~Catapult(); 

	void update(float dt);
	Vec2 attackPos;
	float delay;
	bool attacked;

	map<AnimationType, Entity*> animationMap;
};

