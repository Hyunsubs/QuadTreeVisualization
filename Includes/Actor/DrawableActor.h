#pragma once

#pragma warning (disable : 4251)

#include "Actor.h"
#include <string>

class ENGINE_API DrawableActor : public Actor
{
	// RTTI.
	RTTI_DECLARATIONS(DrawableActor, Actor)

public:
	DrawableActor(const char* image = "");
	DrawableActor(int a, std::string _filepath);
	//virtual ~DrawableActor() = default;
	virtual ~DrawableActor();

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
	virtual void SetPosition(const Vector2& newPosition) override;

	// 충돌 확인 함수.
	bool Intersect(const DrawableActor& other);

	// Getter.
	inline int Width() const { return width; }
	inline int Height() const { return height; }


	// Collision
	virtual void OnCollisionEnter(BoundingBox* _target) override;
	virtual void OnCollision(BoundingBox* _target) override;
	virtual void CollisionEnd(BoundingBox* _target) override;

protected:

	// 화면에 그릴 문자 값.
	std::string image;

	// 너비(처음 \n이 나오기 전까지 길이).
	int width = 0;
	// 높이(전체 길이 / 너비)
	int height = 0;

	bool fixed = false;


	// 색상 값.
	Color color = Color::White;
};