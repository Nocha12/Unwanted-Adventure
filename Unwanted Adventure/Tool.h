#pragma once
#include "Entity.h"
#include "Animation.h"

class Tool : public Entity
{
public:
	Tool(ToolType type);
	~Tool();

	ToolType type;

	float moveRotation;
	float speed;
	
	Animation *animation;

	void update(float dt);
};

