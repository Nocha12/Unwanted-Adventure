#pragma once
#include "Unit.h"
#include "Cave.h"
#include "Timer.h"

class Enemy : public Unit
{
public:
	Enemy();
	~Enemy();

	int scanRange;
	int moveRange;
	int attackRange;
	float attackFin;
};

class Deer : public Enemy
{
public:
	Deer();
	~Deer();

	void update(float dt);
};

class Boar : public Enemy
{
public:
	Boar();
	~Boar();

	bool dash;
	float dashDir;
	float delay;
	
	Timer dashTimer;

	void update(float dt);
};

class Snake : public Enemy
{
public:
	Snake();
	~Snake();
	
	void update(float dt);
};

class Eagle : public Enemy
{
public:
	Eagle();
	~Eagle();

	float delay;

	void update(float dt);
};

class Scorpion : public Enemy
{
public:
	Scorpion();
	~Scorpion();

	void update(float dt);
};