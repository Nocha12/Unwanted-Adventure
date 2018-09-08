#pragma once
class GameManager
{
public:
	GameManager();
	~GameManager();

	int stage;

	int boarCount;
	int eagleCount;
	int enemyCount;
	int	boarKillCount;

	int timeM1;
	int timeS1;
	int timeM2;
	int timeS2;

	void reset();
};

extern GameManager gameManager;