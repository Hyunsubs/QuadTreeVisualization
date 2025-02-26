#pragma once

#include "Core.h"
#include "Vector2.h"

class ENGINE_API Vector2Int
{
public:
	Vector2Int(int x = 0, int y = 0);
	~Vector2Int() = default;

	// 연산자 오버로딩.
	Vector2Int operator+(const Vector2Int& other);
	Vector2Int operator-(const Vector2Int& other);
	bool operator==(const Vector2Int& other);
	bool operator!=(const Vector2Int& other);

	operator Vector2()
	{
		return Vector2((float)x, (float)y);
	}

public:
	// x좌표 / y좌표.
	int x = 0;
	int y = 0;
};