#pragma once
#include <Actor/AnimatedActor.h>
#include <Actors/Player.h>

class BoundingBox;
class Projectile : public AnimatedActor
{
	RTTI_DECLARATIONS(Projectile, AnimatedActor)
public:
	Projectile(bool _left, Vector2& position, PLAYER_STATE state);
	~Projectile();

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	virtual void OnCollisionEnter(BoundingBox* _target) override;
	virtual void OnCollision(BoundingBox* _target) override;
	virtual void CollisionEnd(BoundingBox* _target) override;

private:
	bool left;
	BoundingBox* box;
};