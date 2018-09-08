#pragma once
#include "Unit.h"
#include "Building.h"

class Player : public Unit
{
public:
	Player();
	~Player();

	void update(float dt);
	void render();

	bool isBuilding;
	bool isPicking;
	bool isSleeping;
	bool goToWorkRoom;
	bool goToWell;
	bool goToForge;
	bool goToRaft;
	bool ending;
	bool isCuttingTree;
	bool setTree;
	bool doing;
	float fade;
	float pickingTimer;

	int water;
	int sleep;
	int food;
	float timer;
	float waterEffectTimer;

	bool isBlackOut;
	bool bowAttacked;
	bool crossBowAttacked;
	bool axAttacked;
	bool attackFin;

	Building *building;
	Sprite *getWaterB;
	list<Sprite*> waterEffectList;
};

