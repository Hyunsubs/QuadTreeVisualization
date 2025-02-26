#pragma once
#include <Actor/AnimatedActor.h>

class Player;
class HealthBar : public AnimatedActor
{
	RTTI_DECLARATIONS(HealthBar, AnimatedActor)
public:
	HealthBar();
	~HealthBar();

	inline void SetOwner(Player* _owner) { owner = _owner; }

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

private:
	Player* owner;
	int curHP;

};