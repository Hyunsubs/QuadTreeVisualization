#include "RBTDemoLevel.h"
#include "QuadTree/Bounds.h"
#include "QuadTree/Node.h"
#include "Engine/Engine.h"


RBTDemoLevel::RBTDemoLevel()
{
}

RBTDemoLevel::~RBTDemoLevel()
{
}


void RBTDemoLevel::Init()
{
	qTree = new QuadTree(Bounds(100.f, 100.f, 150.f, 150.f, true));
}

void RBTDemoLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (Engine::Get().GetKey(VK_UP))
	{
		y--;
	}
	if (Engine::Get().GetKey(VK_DOWN))
	{
		y++;
	}
	if (Engine::Get().GetKey(VK_RIGHT))
	{
		x++;
	}
	if (Engine::Get().GetKey(VK_LEFT))
	{
		x--;
	}

	if (Engine::Get().GetKeyDown(VK_LBUTTON))
	{
		Vector2 position = Engine::Get().MousePosition();
		qTree->Insert(new Node(Bounds((float)position.x, (float)position.y, 3.f, 3.f, true, true)));
	}

	if (Engine::Get().GetKeyDown(VK_TAB))
	{
		Engine::Get().SetPrintCollision(!(Engine::Get().GetPrintCollision()));
	}
	// queryNode->Update(x, y);


}

void RBTDemoLevel::Draw()
{
	Super::Draw();
}

