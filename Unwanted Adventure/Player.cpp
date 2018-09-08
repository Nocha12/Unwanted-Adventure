#include "DXUT.h"
#include "Player.h"
#include "MultiDirAnimation.h"
#include "World.h"
#include "GameScene.h"
#include "Arrow.h"
#include "Item.h"
#include "Tree.h"

Player::Player() : timer(0), goToForge(false), goToWell(false), goToWorkRoom(false), fade(1), isSleeping(false), isBuilding(false), isCuttingTree(false), building(nullptr), pickingTimer(0), isPicking(false), setTree(false), water(100), food(100), sleep(100), bowAttacked(false), crossBowAttacked(false), axAttacked(false), attackFin(false), isBlackOut(false), ending(false), goToRaft(false), waterEffectTimer(0)
{
	type = PLAYER;

	addChild(animationMap[IDLE] = new MultiDirAnimation(this, "image/player/idle", 8, 6));
	rect = animationMap[IDLE]->rect;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/player/walk", 12, 8));
	animationMap[WALK]->visible = false;
	addChild(animationMap[ATTACK_AX] = new MultiDirAnimation(this, "image/player/axAttack", 5, 7));
	animationMap[ATTACK_AX]->visible = false;
	addChild(animationMap[ATTACK_BOW] = new MultiDirAnimation(this, "image/player/bow", 7, 7));
	animationMap[ATTACK_BOW]->visible = false;
	addChild(animationMap[ATTACK_CROSSBOW] = new MultiDirAnimation(this, "image/player/crossBow", 5, 7));
	animationMap[ATTACK_CROSSBOW]->visible = false;
	addChild(animationMap[BUILD] = new MultiDirAnimation(this, "image/player/hammer", 5, 6));
	animationMap[BUILD]->visible = false;
	addChild(animationMap[PICK] = new MultiDirAnimation(this, "image/player/pick", 5, 6));
	animationMap[PICK]->visible = false;
	addChild(animationMap[AX] = new MultiDirAnimation(this, "image/player/ax", 5, 6));
	animationMap[AX]->visible = false;
	addChild(animationMap[DEAD] = new MultiDirAnimation(this, "image/player/die", 10, 8, false));
	animationMap[DEAD]->visible = false;
	addChild(animationMap[WATER] = new MultiDirAnimation(this, "image/player/bucket", 10, 8, false));
	animationMap[WATER]->visible = false;

	collisionPos = Vec2(120, 148);
	colRange = 50;
	moveSpeed = 200;
	//stand = true;
	attackFrame = 3;
	attackRange = 180;
	maxHp = hp = 100;
	addChild(getWaterB = new Sprite("image/ui/waterIcon.png"));
	getWaterB->visible = false;
	getWaterB->pos = Vec2(100, 10);
	getWaterB->scaleCenter = getWaterB->rect.center();
}

Player::~Player()
{
}

void Player::render()
{
	Unit::render();

	for_iter(iter, animationMap)
		(*iter).second->render();
}

void Player::update(float dt)
{
	Unit::update(dt);

	GameScene *scene = (GameScene*)world.currentScene;

	if (isDead)
	{
		ChangeAnimation(DEAD);
		return;
	}
	timer += dt;
	
	if (timer >= 2)
	{
		timer = 0;
		sleep = sleep - 1 < 0 ? 0 : sleep - 1;
		food = food - 1 < 0 ? 0 : food - 1;
		water = water - 1 < 0 ? 0 : water - 1;
		
		int count = (sleep == 0 ? 1 : 0) + (food == 0 ? 1 : 0) + (water == 0 ? 1 : 0);
		if (count >= 2)
		{
			canMove = false;
			isDead = true;
			ChangeAnimation(DEAD);
			scene->ui->gameOver->visible = true;
		}
	}

	int sleepT = sleep;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
			if (scene->ui->sleepText[2 - i][j]->visible)
				scene->ui->sleepText[2 - i][j]->visible = false;

		scene->ui->sleepText[2 - i][sleepT % 10]->visible = true;
		sleepT /= 10;
	}

	int foodT = food;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
			if (scene->ui->foodText[2 - i][j]->visible)
				scene->ui->foodText[2 - i][j]->visible = false;

		scene->ui->foodText[2 - i][foodT % 10]->visible = true;
		foodT /= 10;
	}
	
	int waterT = water;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
			if (scene->ui->waterText[2 - i][j]->visible)
				scene->ui->waterText[2 - i][j]->visible = false;

		scene->ui->waterText[2 - i][waterT % 10]->visible = true;
		waterT /= 10;
	}

	if (!canMove && routeList.empty() && isBuilding)
	{
		building->Build(dt);
		ChangeAnimation(BUILD);
	}
	else if (!canMove && routeList.empty() && isSleeping && attackTarget)
	{
		ChangeAnimation(IDLE);

		if (fade > 0 && !isBlackOut)
		{
			fade -= dt;

			for (int i = 0; i < 8; ++i)
				((MultiDirAnimation*)animationMap[IDLE])->animation[i]->color.a = fade;
		}
		else if (scene->ui->blackOut->color.a < 1 && !isBlackOut)
			scene->ui->blackOut->color.a += dt * 0.5f;
		else if (scene->ui->blackOut->color.a >= 1 && !isBlackOut)
		{
			isBlackOut = true;
			sleep = 100;
		}
		else if (scene->ui->blackOut->color.a > 0 && isBlackOut)
			scene->ui->blackOut->color.a -= dt;
		else if (scene->ui->blackOut->color.a <= 0)
		{
			if (fade < 1)
			{
				fade += dt;

				for (int i = 0; i < 8; ++i)
					((MultiDirAnimation*)animationMap[IDLE])->animation[i]->color.a = fade;
			}
			else if (fade >= 1)
			{
				isSleeping = false;
				canMove = true;
				isBlackOut = false;
				attackTarget = nullptr;
			}
		}
	}
	else if (!canMove && routeList.empty() && goToWorkRoom && attackTarget)
	{
		goToWorkRoom = false;
		canMove = true;
		if (scene->ui->UseItem(BRANCH_ITEM, true, 3))
		{
			Item *item = new Item(ROPE_ITEM);
			scene->ui->makeItemList.push_back(item);
			scene->ui->MakeItem(attackTarget->GetCollisionPos());
		}
		attackTarget = nullptr;
	}
	else if (!canMove && routeList.empty() && goToWell && attackTarget)
	{
		goToWell = false;
		canMove = true;
		Item *item = new Item(WATERBUCKET_ITEM);
		scene->ui->makeItemList.push_back(item);
		scene->ui->MakeItem(attackTarget->GetCollisionPos());
		attackTarget = nullptr;
	}
	else if (!canMove && routeList.empty() && goToForge && attackTarget)
	{
		goToForge = false;
		canMove = true;
		scene->ui->ViewWeaponPage();
		attackTarget = nullptr;
	}
	else if (!canMove && routeList.empty() && goToRaft && attackTarget)
	{
		goToRaft = false;
		canMove = true;
		attackTarget = nullptr;
		ending = true;
	}
	else if (!canMove && routeList.empty() && isPicking && attackTarget)
	{
		pickingTimer += dt;

		if (pickingTimer > 1)
		{
			pickingTimer = 0;
			isPicking = false;
			canMove = true;
			for (int i = 0; i < random(2, 5); i++)
			{
				Item *item = new Item(ROCK_ITEM);
				scene->ui->makeItemList.push_back(item);
			}
			scene->ui->MakeItem(attackTarget->GetCollisionPos());

			((Building*)attackTarget)->onDead();
		}
		ChangeAnimation(PICK);
	}
	else if (!canMove && routeList.empty() && isCuttingTree && attackTarget)
	{
		if (!setTree)
		{
			setTree = true;
			((Tree*)attackTarget)->logBtn->visible = true;
			((Tree*)attackTarget)->leafBtn->visible = true;
			((Tree*)attackTarget)->branchBtn->visible = true;
			((Tree*)attackTarget)->orangeBtn->visible = true;
		}
		ChangeAnimation(AX);
	}
	if (attackTarget && (attackTarget->isUnit || attackTarget->type == CAVE))
	{
		if (pita(GetCollisionPos(), attackTarget->GetCollisionPos()) > attackRange && attackFin)
		{
			SetWalk(attackTarget->currentTile->GetCenter());
		}

		if (attackTarget->type == DEER && (scene->ui->toolSprite[BOW_ITEM]->visible || !scene->ui->arrowSprite->visible))
		{
			attackRange = 180;
			if (pita(GetCollisionPos(), attackTarget->GetCollisionPos()) <= attackRange)
			{
				routeList.clear();
				ChangeAnimation(ATTACK_AX);
				moveRotation = angle(GetCollisionPos(), attackTarget->GetCollisionPos());
			}
		}
		else if (scene->ui->toolSprite[CROSSBOW_ITEM]->visible && !scene->ui->toolSprite[BOW_ITEM]->visible)
		{
			attackRange = 500;
			if (pita(GetCollisionPos(), attackTarget->GetCollisionPos()) <= attackRange)
			{
				routeList.clear();
				ChangeAnimation(ATTACK_BOW);
				moveRotation = angle(GetCollisionPos(), attackTarget->GetCollisionPos());
			}
		}
		else if(!scene->ui->toolSprite[CROSSBOW_ITEM]->visible)
		{
			attackRange = 800;
			if (pita(GetCollisionPos(), attackTarget->GetCollisionPos()) <= attackRange)
			{
				routeList.clear();
				ChangeAnimation(ATTACK_CROSSBOW);
				moveRotation = angle(GetCollisionPos(), attackTarget->GetCollisionPos());
			}
		}
	}

	MultiDirAnimation *attackAni = (MultiDirAnimation*)animationMap[ATTACK_BOW];

	if (attackTarget && (int)attackAni->currentFrame == attackFrame && !bowAttacked)
	{
		Arrow *arrow = new Arrow(angle(center(), attackTarget->GetCollisionPos()), 600, 1, 50);
		arrow->setCenter(center());
		arrow->rotation += D3DXToRadian(90);
		scene->arrowList.push_back(arrow);
		scene->addChild(arrow);
		bowAttacked = true;
	}
	else if (!((int)attackAni->currentFrame == attackFrame))
		bowAttacked = false;

	if (attackAni->visible && (int)attackAni->currentFrame == attackAni->animation[0]->textures.size() - 1)
		attackFin = true;
	else if (attackAni->visible)
		attackFin = false;

	MultiDirAnimation *attackCrossBowAni = (MultiDirAnimation*)animationMap[ATTACK_CROSSBOW];

	if (attackTarget && (int)attackCrossBowAni->currentFrame == attackFrame && !crossBowAttacked)
	{
		Arrow *arrow = new Arrow(angle(center(), attackTarget->GetCollisionPos()), 800, 1, 70);
		arrow->setCenter(center());
		arrow->rotation += D3DXToRadian(90);
		scene->arrowList.push_back(arrow);
		scene->addChild(arrow);
		crossBowAttacked = true;
	}
	else if (!((int)attackCrossBowAni->currentFrame == attackFrame))
		crossBowAttacked = false;

	if (attackCrossBowAni->visible && (int)attackCrossBowAni->currentFrame == attackCrossBowAni->animation[0]->textures.size() - 1)
		attackFin = true;
	else if (attackCrossBowAni->visible)
		attackFin = false;

	MultiDirAnimation *axAttackAni = (MultiDirAnimation*)animationMap[ATTACK_AX];

	if (attackTarget && (int)axAttackAni->currentFrame == 2 && !axAttacked)
	{
		((Unit*)attackTarget)->Attacked(angle(GetCollisionPos(), attackTarget->GetCollisionPos()), 50);
		axAttacked = true;
	}
	else if (!((int)axAttackAni->currentFrame == 2))
		axAttacked = false;
	
	if (axAttackAni->visible && (int)axAttackAni->currentFrame == axAttackAni->animation[0]->textures.size() - 1)
		attackFin = true;
	else if (axAttackAni->visible)
		attackFin = false;

	if (!attackAni->visible && !axAttackAni->visible && !attackCrossBowAni->visible)
		attackFin = true;

	if (currentTile->code == WATER_TILE && canMove && !scene->ui->toolSprite[BUCKET_ITEM]->visible)
		getWaterB->visible = true;
	else
		getWaterB->visible = false;

	Vec2 mousePos = world.getMousePos() - scene->pos - pos;
	if (getWaterB->rectWithPos().intersects(mousePos) && getWaterB->visible)
	{
		getWaterB->scale = Vec2(1.2f, 1.2f);
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			canMove = false;
			routeList.clear();
			ChangeAnimation(WATER, true);
			return;
		}
	}
	else
		getWaterB->scale = Vec2(1, 1);
	MultiDirAnimation *waterAni = (MultiDirAnimation*)animationMap[WATER];
	if (waterAni->visible && (int)waterAni->currentFrame == waterAni->animation[0]->textures.size() - 1)
	{
		canMove = true;
		Item *item = new Item(WATERBUCKET_ITEM);
		scene->ui->makeItemList.push_back(item);
		scene->ui->MakeItem(GetCollisionPos());
	}

	if (currentTile->code == WATER_TILE && waterEffectTimer >= 0.3f)
	{
		waterEffectTimer = 0;
		Sprite *spr = new Sprite("image/circle.png");
		spr->scaleCenter = spr->rect.center();
		addChild(spr);
		spr->setCenter(collisionPos);
		waterEffectList.push_back(spr);
	}
	else if (currentTile->code != WATER_TILE)
	{
		for_iter(iter, waterEffectList)
			(*iter)->visible = false;
	}
	waterEffectTimer += dt;
	for_iter(iter, waterEffectList)
	{
		(*iter)->scale += Vec2(dt * 2, dt * 2);
		(*iter)->color.a -= dt;
		if ((*iter)->color.a <= 0)
		{
			removeChild(*iter);
			iter = waterEffectList.erase(iter);
			if (iter == waterEffectList.end()) break;
		}
	}
}
