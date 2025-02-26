#pragma once

#include "Core.h"
#include "RTTI.h"
#include <vector>

// 전방 선언(Forward Declaration).
class Actor;
class ENGINE_API Level : public RTTI
{
	// RTTI 정의.
	RTTI_DECLARATIONS(Level, RTTI)

public:
	Level();
	virtual ~Level();

	// 액터 추가 함수.
	void AddActor(Actor* newActor);

	// 삭제 요청이 된 액터를 정리하는 함수.
	//void DestroyActor();
	void ProcessAddedAndDestroyedActor();

	// 초기화 함수(처음 세팅할것들)
	virtual void Init();

	std::vector<Actor*>& GetActorList() { return actors; }

	// 루프 처리 함수.
	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void AfterDraw();

	void SetAfterDraw(bool afterDraw) { this->afterDraw = afterDraw; }
	bool GetAfterDraw() { return afterDraw; }

protected:
	// 게임 공간에 배치되는 물체(액터) 배열.
	std::vector<Actor*> actors;
	std::vector<Actor*> requestedActor;

	// 추가 요청된 액터.
	Actor* addRequestedActor = nullptr;
	bool afterDraw = false;
};