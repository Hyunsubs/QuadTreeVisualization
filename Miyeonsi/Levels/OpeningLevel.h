#pragma once
#include "Level/Level.h"

class SoundObject;
class OpeningLevel : public Level
{
	RTTI_DECLARATIONS(OpeningLevel, Level)
public:
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;


	OpeningLevel();
	~OpeningLevel();


private:
	SoundObject* openingSound = nullptr;
	bool started = false;
	class Timer* openingTimer = nullptr;
	class Timer* flasher = nullptr;
	class DrawableActor* title = nullptr;
	class DrawableActor* pressStart = nullptr;
};