#pragma once
#include "Level/Level.h"
#include "QuadTree/QuadTree.h"

class RBTDemoLevel : public Level
{
	RTTI_DECLARATIONS(RBTDemoLevel, Level)
public:
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	RBTDemoLevel();
	~RBTDemoLevel();


private:
	QuadTree* qTree = nullptr;
	class Node* queryNode = nullptr;
	int x = 0;
	int y = 0;
};