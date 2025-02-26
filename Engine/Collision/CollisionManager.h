#pragma once
#include "Core.h"
#include <vector>
#include "Actor/BoundingBox.h"


class Level;
class Actor;
class ENGINE_API CollisionManager
{
public:
	inline void SetCurrentLevel(Level* level) { cur_level = level; }
	void Update();
	void SetCollision(LAYER _first, LAYER _second);
	void CheckCollision(LAYER _first, LAYER _second);

	CollisionManager();
	~CollisionManager();

private:
	std::vector<std::vector<BoundingBox*>> boxes = {};
	bool check_flag[(int)LAYER::NONE][(int)LAYER::NONE] = {};
	Level* cur_level = nullptr;
	
};