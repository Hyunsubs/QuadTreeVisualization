#include "PreCompiledHeader.h"
#include "Actor.h"

#include "Core.h"

Actor::Actor()
	: isActive(true), isExpired(false)
{
}

Actor::~Actor()
{
}

void Actor::Init()
{

}

void Actor::Update(float delatTime)
{
}

void Actor::Draw()
{
}


void Actor::SetPosition(const Vector2& newPosition)
{
	position = newPosition;
}

inline Vector2 Actor::Position() const
{
	return position;
}

void Actor::OnCollisionEnter(BoundingBox* _target)
{
}

void Actor::OnCollision(BoundingBox* _target)
{
}

void Actor::CollisionEnd(BoundingBox* _target)
{
}

