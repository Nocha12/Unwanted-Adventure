#pragma once
#include "Entity.h"
#include "Sprite.h"

class Arrow :
	public Entity
{
public:
	Arrow(float dir, float speed, float timer, int damage);
	~Arrow();

	float dir;
	float speed;
	float timer;
	int damage;
	float fade;

	Sprite *sprite;

	void update(float dt);
};

