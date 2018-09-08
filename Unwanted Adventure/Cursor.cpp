#include "DXUT.h"
#include "Cursor.h"
#include "GameScene.h"
#include "World.h"
#include "Unit.h"

Cursor::Cursor()
{
}

Cursor::~Cursor()
{
}

void Cursor::update(float dt)
{
	Entity::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;

	if (world.getKeyState(VK_LBUTTON) == 1)
	{
		if (scene->waitBuild != nullptr && scene->waitBuild->canBuild)
		{
			scene->waitBuild->buildMode = false;  
			scene->objList.push_back(scene->waitBuild);
			scene->waitBuild->instantBuild();
			scene->waitBuild = nullptr;
		}
	}

	if (!scene->player->canMove)
		scene->ChangeCursor(X_CURSOR);
	else
	{
		bool check = true;
		for_iter(iter, scene->objList)
		{
			if (collision((*iter)->pos, world.getMousePos() - scene->pos, (*iter)->rect) && (*iter)->type != PLAYER)
			{
				if ((*iter)->type == HOME)
					scene->ChangeCursor(SLEEP_CURSOR);
				else if ((*iter)->type == ROCK && !scene->ui->toolSprite[PICK_ITEM]->visible)
					scene->ChangeCursor(PICK_CURSOR);
				else if ((*iter)->type == ROCK)
					scene->ChangeCursor(X_CURSOR);
				else if ((*iter)->type == WORKROOM && scene->ui->UseItem(BRANCH_ITEM, false, 3))
					scene->ChangeCursor(HAMMER_CURSOR);
				else if ((*iter)->type == WORKROOM)
					scene->ChangeCursor(X_CURSOR);
				else if ((*iter)->type == TREE && scene->ui->toolSprite[AX_ITEM]->visible)
					scene->ChangeCursor(X_CURSOR);
				else if ((*iter)->type == TREE)
					scene->ChangeCursor(AX_CURSOR);
				else if ((*iter)->isUnit && (*iter)->type == DEER && scene->ui->toolSprite[AX_ITEM]->visible)
					scene->ChangeCursor(X_CURSOR);
				else if ((*iter)->isUnit && (*iter)->type == DEER && !scene->ui->toolSprite[AX_ITEM]->visible)
					scene->ChangeCursor(ATTACK_CURSOR);
				else if ((*iter)->isUnit && !((*iter)->type == DEER) && (scene->ui->toolSprite[BOW_ITEM]->visible || !scene->ui->arrowSprite->visible))
					scene->ChangeCursor(X_CURSOR);
				else if ((*iter)->isUnit)
					scene->ChangeCursor(ATTACK_CURSOR);
				check = false;
			}
		}
		if (check)
			scene->ChangeCursor(MOVE_CURSOR);
	}

	if (world.getKeyState(VK_RBUTTON) == 1 && scene->player->canMove)
	{
		Object *target = nullptr;

		scene->player->attackTarget = nullptr;

		for_iter(iter, scene->objList)
		{
			if (collision((*iter)->pos, world.getMousePos() - scene->pos, (*iter)->rect) && (*iter)->type != PLAYER)
			{
				target = *iter;
			}
		}
		if (target)
		{
			scene->player->routeList.clear();
			scene->player->attackTarget = target;
			if (target->isUnit)
			{
				if (((Unit*)target)->isDead)
				{
					scene->player->attackTarget = nullptr;
					return;
				}

				if (target->type == DEER && scene->ui->toolSprite[AX_ITEM]->visible)
				{
					scene->player->attackTarget = nullptr;
					scene->player->SetWalk(world.getMousePos() - scene->pos);
				}
				if (!(target->type == DEER) && (scene->ui->toolSprite[BOW_ITEM]->visible || !scene->ui->arrowSprite->visible))
				{
					scene->player->attackTarget = nullptr;
					scene->player->SetWalk(world.getMousePos() - scene->pos);
				}
			}
			if (!target->isUnit && ((Building*)target)->sprite->color.a == 1)
			{
				scene->player->SetWalk(world.getMousePos() - scene->pos);
				if (target->type == ROCK && !scene->ui->toolSprite[PICK_ITEM]->visible)
				{
					scene->player->canMove = false;
					scene->player->isPicking = true;
					scene->player->SetWalk(target->currentTile->GetCenter());
				}
				else if (target->type == TREE && !scene->ui->toolSprite[AX_ITEM]->visible)
				{
					scene->player->canMove = false;
					scene->player->isCuttingTree = true;
					scene->player->SetWalk(target->currentTile->GetCenter());
				}
				else if (target->type == HOME)
				{
					scene->player->canMove = false;
					scene->player->isSleeping = true;
					scene->player->SetWalk(target->currentTile->GetCenter());
				}
				else if (target->type == RAFT && scene->caveList.empty())
				{
					scene->player->canMove = false;
					scene->player->goToRaft = true;
					scene->player->SetWalk(target->currentTile->GetCenter());
				}
				else if (target->type == WORKROOM)
				{
					scene->player->canMove = false;
					scene->player->goToWorkRoom = true;
					scene->player->SetWalk(target->currentTile->GetCenter());
				}
				else if (target->type == WELL)
				{
					scene->player->canMove = false;
					scene->player->goToWell = true;
					scene->player->SetWalk(target->currentTile->GetCenter());
				}
				else if (target->type == FORGE)
				{
					scene->player->canMove = false;
					scene->player->goToForge = true;
					scene->player->SetWalk(target->currentTile->GetCenter());
				}
				else if (target->type == CAVE && (scene->ui->toolSprite[CROSSBOW_ITEM]->visible || !scene->ui->arrowSprite->visible))
				{
					scene->player->SetWalk(world.getMousePos() - scene->pos);
					scene->player->attackTarget = nullptr;
				}
			}
		}
		else
			scene->player->SetWalk(world.getMousePos() - scene->pos);
	}
}
