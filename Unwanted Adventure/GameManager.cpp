#include "DXUT.h"
#include "GameManager.h"

GameManager::GameManager() : stage(1), boarCount(0), eagleCount(0), enemyCount(0), boarKillCount(0), timeM1(0), timeM2(0), timeS1(0), timeS2(0)
{
}

GameManager::~GameManager()
{
}

void GameManager::reset()
{
	stage = 1; boarCount = 0; eagleCount = 0; enemyCount = 0; boarKillCount = 0;
	timeM1 = 0; timeM2 = 0; timeS1 = 0; timeS2 = 0;
}

GameManager gameManager;
