#pragma once
#include "Level\Level.h"

class Player;
class TitleLevel : public Level
{
	RTTI_DECLARATIONS(TitleLevel, Level)
public:
	void Init();
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;


	TitleLevel();
	~TitleLevel();


private:
	bool is_sound = false;
	Player* players[2] = {};
	class SoundObject* testSound = nullptr;

};