#pragma once

#include "Engine/Engine.h"
#include "Level/Level.h"
#include "Actor/BoundingBox.h"

class ENGINE_API Bounds
{
public:
	Bounds(float x, float y, float width = 1.f, float height = 1.f, bool isRoot = false, bool isRed = false)
		: x(x), y(y), width(width), height(height)
	{
		this->isRoot = isRoot;
		newBox = new BoundingBox(width, height);
		newBox->SetPosition(Vector2(x, y));
		newBox->SetRoot(isRoot);
		if (isRed)
			newBox->SetColor(Color::Red);
		Level* curLevel = Engine::Get().GetCurrentLevel();
		curLevel->AddActor(newBox);
	}



	// Getter
	float X() const { return x; }
	float Y() const { return y; }
	float Width() const { return width; }
	float Height() const { return height; }

	float MaxX() const { return x + width / 2.f; }
	float MaxY() const { return y + height / 2.f; }

	void SetX(float x) { this->x = x; }
	void SetY(float y) { this->y = y; }

	// ∞„ƒß ∆«¡§ «‘ºˆ
	bool Intersects(const Bounds& other) const;
	void Update(float x, float y) 
	{ 
		if (newBox)
		{
			newBox->SetPosition(Vector2(x, y));
		}
	}

	BoundingBox* GetBoundingBox() 
	{
		if (newBox)
			return newBox;
		else
			return nullptr;
	}

private:
	// ¡ﬂæ” ±‚¡ÿ
	float x = 0.f;
	float y = 0.f;
	float width = 1.f;
	float height = 1.f;
	bool isRoot = false;
	BoundingBox* newBox = nullptr;
};
