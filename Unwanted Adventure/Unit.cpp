#include "DXUT.h"
#include "Unit.h"
#include "MultiDirAnimation.h"
#include "GameManager.h"
#include "GameScene.h"
#include "World.h"

Unit::Unit() : moveSpeed(0), canMove(true), stand(false), isAttacked(false), attacked(false), backAngle(0), backSpeed(0), attackFrame(0), attackRange(0), isDead(false)
{
	isUnit = true;
}

Unit::~Unit()
{
}

void Unit::update(float dt)
{
	Object::update(dt);
	if (isDead)
		return;

	GameScene *scene = (GameScene*)world.currentScene;

	if (isAttacked)
	{
		routeList.clear();
		pos += Vec2(cos(backAngle), sin(backAngle)) * backSpeed * dt;
		 
		backSpeed -= dt * moveSpeed * 2;

		if (backSpeed <= 0)
			isAttacked = false;
	}
	else
	{
		for_iter(obj, scene->objList)
		{
			if (*obj == this) continue;

			if (circleCollision(GetCollisionPos(), (*obj)->GetCollisionPos(), colRange + (*obj)->colRange))
			{
				float t = angle(GetCollisionPos(), (*obj)->GetCollisionPos());

				if (!stand)
					pos -= Vec2(cosf(t), sinf(t)) * moveSpeed * dt;
				if (scene->GetTile(GetCollisionPos()) == scene->GetTile(movePos)){
					if (routeList.size() > 0){
						routeList.pop_front();
						if (routeList.size() > 0){
							Tile* t = routeList.front();
							if (t->unitTile){
								if (t->target){
									astar(routeList.back());
								}
							}
							else{
								movePos = t->GetCenter();
							}
						}
					}
					else{
						ChangeAnimation(IDLE);
						return;
					}
				}
			}
		}
		Move(dt);
	}

	SetCurrentTile();
}

void Unit::ChangeAnimation(AnimationType type, bool resetAni)
{
	for_iter(ani, animationMap)
	{
		if ((*ani).first == type) continue;

		(*ani).second->visible = false;
		((MultiDirAnimation*)(*ani).second)->Reset();
		((MultiDirAnimation*)(*ani).second)->Stop();
	}

	if (resetAni)
		((MultiDirAnimation*)animationMap[type])->Reset();

	animationMap[type]->visible = true;

	((MultiDirAnimation*)animationMap[type])->Play();
}

void Unit::Move(float dt)
{
	GameScene* GS = (GameScene*)world.currentScene;

	if (routeList.empty()) {
		if (pita(GetCollisionPos(), finalMovePos) > 10)
			moveRotation = angle(GetCollisionPos(), finalMovePos);
		if (canMove && (!attackTarget || (!attackTarget->isUnit && !(attackTarget->type == CAVE))))
			ChangeAnimation(IDLE);

		return;
	}

	float dir = angle(GetCollisionPos(), movePos);
	pos += Vec2(cos(dir), sin(dir)) * moveSpeed * dt;

	float distance = pita(GetCollisionPos(), movePos);
	if (distance < moveSpeed / 30){
		SetCollisionPos(movePos);
		if (routeList.size() > 0){
			routeList.pop_front();
			if (routeList.size() > 0){
				Tile* t = routeList.front();
				if (t->unitTile){
					if (t->target){
						astar(routeList.back());
					}
				} 
				else{
					movePos = t->GetCenter();
				}
			}
		}
		else if(!attackTarget) {
			ChangeAnimation(IDLE);
			return;
		}
	}

	moveRotation = dir;
}

void Unit::astar(Tile* target)
{
	GameScene *GS = (GameScene*)world.currentScene;

	if (currentTile == nullptr) return;
	if (target->blocked)
	{
		ChangeAnimation(IDLE);
		return;
	}

	Tile *nowTile = currentTile;
	bool closeListArray[TILEWIDTH][TILEHEIGHT] = { 0, };
	bool openListArray[TILEWIDTH][TILEHEIGHT] = { 0, };

	list<Tile *> openList;
	list<Tile *> closeList;

	openList.push_back(nowTile);
	function<void(Tile*)> searchTile = [&](Tile *t)
	{
		Tile *searchTileArray[8];
		for (int i = 0; i < 8; i++) searchTileArray[i] = nullptr;

		searchTileArray[0] = GS->mapTile[(int)t->tilePos.x][(int)t->tilePos.y - 2];
		searchTileArray[1] = GS->mapTile[(int)t->tilePos.x][(int)t->tilePos.y - 1];
		searchTileArray[2] = GS->mapTile[(int)t->tilePos.x + 1][(int)t->tilePos.y];

		if ((int)t->tilePos.y % 2 == 0){
			searchTileArray[3] = GS->mapTile[(int)t->tilePos.x - 1][(int)t->tilePos.y - 1];
		}
		else{
			searchTileArray[3] = GS->mapTile[(int)t->tilePos.x + 1][(int)t->tilePos.y + 1];
		}

		searchTileArray[4] = GS->mapTile[(int)t->tilePos.x][(int)t->tilePos.y + 1];

		searchTileArray[5] = GS->mapTile[(int)t->tilePos.x - 1][(int)t->tilePos.y];
		if ((int)t->tilePos.y % 2 == 0){
			searchTileArray[6] = GS->mapTile[(int)t->tilePos.x - 1][(int)t->tilePos.y + 1];
		}
		else{
			searchTileArray[6] = GS->mapTile[(int)t->tilePos.x + 1][(int)t->tilePos.y - 1];
		}

		searchTileArray[7] = GS->mapTile[(int)t->tilePos.x][(int)t->tilePos.y + 2];

		for (int i = 0; i < 8; i++)
		{
			if (searchTileArray[i] == nullptr) continue;
			if (searchTileArray[i]->blocked) continue;

			if (searchTileArray[i] == target)
			{
				searchTileArray[i]->parentTile = t;
				openListArray[(int)searchTileArray[i]->tilePos.x][(int)searchTileArray[i]->tilePos.y] = true;
				openList.push_front(searchTileArray[i]);
				return;
			}
			
			if (searchTileArray[i]->unitTile || searchTileArray[i]->buildTile) {
				continue;
			}

			if (closeListArray[(int)searchTileArray[i]->tilePos.x][(int)searchTileArray[i]->tilePos.y]) continue;
			if (openListArray[(int)searchTileArray[i]->tilePos.x][(int)searchTileArray[i]->tilePos.y])
			{
				if (searchTileArray[i]->GetAllParentCost() > t->GetAllParentCost() + t->GetParentCost(searchTileArray[i]))
					searchTileArray[i]->parentTile = t;
				continue;
			}

			searchTileArray[i]->parentTile = t;
			openListArray[(int)searchTileArray[i]->tilePos.x][(int)searchTileArray[i]->tilePos.y] = true;
			openList.push_front(searchTileArray[i]);
		}
	};

	while (target != nowTile)
	{
		openList.remove(nowTile);
		openListArray[(int)nowTile->tilePos.x][(int)nowTile->tilePos.y] = false;

		closeList.push_back(nowTile);
		closeListArray[(int)nowTile->tilePos.x][(int)nowTile->tilePos.y] = true;

		searchTile(nowTile);
		if (openList.size() == 0)
		{
			for_iter(iter, openList) (*iter)->parentTile = nullptr;
			for_iter(iter, closeList) (*iter)->parentTile = nullptr;
			ChangeAnimation(IDLE);
			return;
		}

		nowTile = openList.front();

		for_iter(iter, openList)
		{
			if ((*iter)->GetAllCost(target) < nowTile->GetAllCost(target)) nowTile = *iter;
		}
	}

	routeList.clear();

	while (nowTile->parentTile != nullptr)
	{
		routeList.push_front(nowTile);
		nowTile = nowTile->parentTile;
	}

	for_iter(iter, openList) (*iter)->parentTile = nullptr;
	for_iter(iter, closeList) (*iter)->parentTile = nullptr;

	if (!routeList.empty() && (routeList.back()->unitTile || routeList.back()->buildTile)){
		routeList.pop_back();
	}

	if (routeList.size() == 0)
	{
		ChangeAnimation(IDLE);
		return;
	}
	
	movePos = routeList.front()->GetCenter();
	return;
}

void Unit::SetCurrentTile()
{
	GameScene* GS = (GameScene*)world.currentScene;
	if (currentTile != nullptr){
		if (currentTile->code == WATER_TILE)
			pos.y -= 10;
		currentTile->unitTile = false;
		currentTile->target = nullptr;
	}

	currentTile = GS->GetTile(GetCollisionPos());
	currentTile->unitTile = true;
	currentTile->target = this;
	if (currentTile->code == WATER_TILE)
		pos.y += 10;
}

void Unit::SetWalk(Vec2 movePos)
{
	routeList.clear();
	this->movePos = movePos;
	finalMovePos = ((GameScene*)world.currentScene)->GetTile(movePos)->GetCenter();
	ChangeAnimation(WALK);
	astar(((GameScene*)world.currentScene)->GetTile(movePos));
}

void Unit::Attacked(float backAngle, float backSpeed, int damage)
{
	if (isDead) return;
	GameScene* scene = (GameScene*)world.currentScene;

	this->backAngle = backAngle;
	this->backSpeed = backSpeed;
	isAttacked = true;

	hp = hp - damage < 0 ? 0 : hp - damage;

	if (type == PLAYER)
		scene->ui->redEffect->color.a = 1;

	if (hp == 0)
	{
		if (type == DEER)
		{
			for (int i = 0; i < random(1, 2); i++)
			{
				Item *item = new Item(SKIN_ITEM);
				scene->ui->makeItemList.push_back(item);
			}
			for (int i = 0; i < random(1, 2); i++)
			{
				Item *item = new Item(MEAT_ITEM);
				scene->ui->makeItemList.push_back(item);
			}
			scene->ui->MakeItem(GetCollisionPos());
		}
		else
		{
			scene->player->hp = scene->player->hp + 5 > 100 ? 100 : scene->player->hp + 5;
			gameManager.enemyCount--;
			if (type == BOAR)
				gameManager.boarKillCount++;
		}
		for_iter(obj, scene->objList)
		if ((*obj)->attackTarget == this)
			(*obj)->attackTarget = nullptr;

		if (type == PLAYER)
		{
			canMove = false;
			isDead = true;
			ChangeAnimation(DEAD, true);
			scene->ui->gameOver->visible = true;
			return;
		}

		for_iter(ani, animationMap)
			((MultiDirAnimation*)ani->second)->FaidOut();

		deadTimer.reset(0.5f, 1);
		deadTimer.onFinished = [=](){
			for_iter(obj, scene->objList)
			if ((*obj)->attackTarget == this)
				(*obj)->attackTarget = nullptr;
			scene->objList.remove(this);
			removeChild(this);
			currentTile->unitTile = false;
		};
		isDead = true;
	}

	for_iter(ani, animationMap)
		((MultiDirAnimation*)ani->second)->GetRed();
}