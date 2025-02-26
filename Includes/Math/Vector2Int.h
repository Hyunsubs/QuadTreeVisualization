#pragma once

#include "Core.h"
#include "Vector2.h"

class ENGINE_API Vector2Int
{
public:
	Vector2Int(int x = 0, int y = 0);
	~Vector2Int() = default;

	// ������ �����ε�.
	Vector2Int operator+(const Vector2Int& other);
	Vector2Int operator-(const Vector2Int& other);
	bool operator==(const Vector2Int& other);
	bool operator!=(const Vector2Int& other);

	operator Vector2()
	{
		return Vector2((float)x, (float)y);
	}

public:
	// x��ǥ / y��ǥ.
	int x = 0;
	int y = 0;
};