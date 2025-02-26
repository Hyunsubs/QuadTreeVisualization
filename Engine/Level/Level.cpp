#include "PreCompiledHeader.h"
#include "Level.h"
#include "Actor/Actor.h"
#include "Actor/BoundingBox.h"
#include "Engine/Engine.h"

Level::Level()
{
}

Level::~Level()
{
	// 메모리 해제.
	for (Actor* actor : actors)
	{
		delete actor;
	}
}

void Level::AddActor(Actor* newActor)
{
	requestedActor.push_back(newActor);
}


void Level::ProcessAddedAndDestroyedActor()
{
	// 액터 순회 후 삭제 요청된 액터를 처리.
	for (int ix = 0; ix < actors.size();)
	{
		if (actors[ix]->isExpired)
		{
			delete actors[ix];
			actors[ix] = nullptr;
			actors.erase(actors.begin() + ix);
			continue;
		}
		++ix;
	}

	// 추가 요청된 액터 처리.
	for (auto& actor: requestedActor)
	{
		actors.push_back(actor);
	}

	requestedActor.clear();
}

void Level::Init()
{
	for (Actor* actor : actors)
	{
		if (!actor->isActive || actor->isExpired)
		{
			continue;
		}

		actor->Init();
	}
}

void Level::Update(float deltaTime)
{
	// 레벨에 포함된 액터를 순회하면서 Update 함수 호출.
	for (Actor* actor : actors)
	{
		// 액터가 비활성화 상태이거나, 삭제 요청된 경우 건너뛰기.
		if (actor == nullptr)
			continue;

		if (!actor->isActive || actor->isExpired)
		{
			continue;
		}

		actor->Update(deltaTime);
	}
}

void Level::Draw()
{
	if (Engine::Get().GetPrintCollision())
	{
		std::vector<Actor*> boundingBoxes = {};
		// 레벨에 포함된 액터를 순회하면서 Draw 함수 호출.
		for (Actor* actor : actors)
		{
			// 액터가 비활성화 상태이거나, 삭제 요청된 경우 건너뛰기.
			if (!actor->isActive || actor->isExpired)
			{
				continue;
			}
			else if (actor->As<BoundingBox>())
			{
				boundingBoxes.push_back(actor);
				continue;
			}
			actor->Draw();
		}

		for (int i = 0; i < boundingBoxes.size(); i++)
		{
			boundingBoxes[i]->Draw();
		}
	}

	else
	{
		// 레벨에 포함된 액터를 순회하면서 Draw 함수 호출.
		for (Actor* actor : actors)
		{
			// 바운딩 박스인 경우 프린트 하지 않음
			if (actor->As<BoundingBox>())
			{
				continue;
			}
			// 액터가 비활성화 상태이거나, 삭제 요청된 경우 건너뛰기.
			if (!actor->isActive || actor->isExpired)
			{
				continue;
			}
			actor->Draw();
		}
	}

}

void Level::AfterDraw()
{

}
