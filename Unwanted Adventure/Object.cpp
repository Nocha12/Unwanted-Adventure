#include "DXUT.h"
#include "Object.h"

Object::Object() : moveRotation(0), attackTarget(nullptr), currentTile(nullptr), colRange(0), isUnit(false), hpBar(nullptr), hpBarBack(nullptr), hp(0), maxHp(0)
{
}

Object::~Object()
{
}

Vec2 Object::GetCollisionPos(){ return pos + collisionPos; }
void Object::SetCollisionPos(Vec2 pos){ this->pos = pos - collisionPos; }

void Object::update(float dt)
{
	Entity::update(dt);

	if (hpBar)
		hpBar->visibleRect.right = hpBar->rect.right * hp / maxHp;
}