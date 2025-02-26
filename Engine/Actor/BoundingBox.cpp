#include "PreCompiledHeader.h"
#include "BoundingBox.h"
#include "Engine/Engine.h"

BoundingBox::BoundingBox()
	: Super()
{
	color = Color::Green;
}

BoundingBox::BoundingBox(int width, int height)
	: width(width), height(height)
{
	SetSize(width, height);
	color = Color::Green;
}

BoundingBox::BoundingBox(float width, float height)
{
	SetSize((int)width, (int)height);
	color = Color::Green;
}

BoundingBox::BoundingBox(int width, int height, bool isRoot)
{
	SetSize(width, height);
	color = Color::Green;
	this->isRoot = isRoot;
}

BoundingBox::BoundingBox(float width, float height, bool isRoot)
{
	SetSize((int)width, (int)height);
	color = Color::Green;
	this->isRoot = isRoot;
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::SetSize(int _width, int _height)
{
	image_buffer.clear();

	width = _width;
	height = _height;

	for (int i = 1; i <= height; i++)
	{
		for (int j = 1; j <= width + 1; j++)
		{
			if (j == width + 1)
				image_buffer += '\n';
			else if (j == 1 || i == 1 || j == width || i == height - 1)
				image_buffer += '$';
			else
				image_buffer += ' ';
		}
	}

	image = image_buffer;
}


void BoundingBox::OnCollisionEnter(BoundingBox* _target)
{
	if (owner == nullptr)
	{
		return;
	}

	if (owner && owner->IsActive() && _target && _target->IsActive())
	{
		owner->OnCollisionEnter(_target);
	}
}

void BoundingBox::OnCollision(BoundingBox* _target)
{
	if (owner == nullptr)
	{
		return;
	}

	if (owner && owner->IsActive() && _target && _target->IsActive())
	{
		owner->OnCollision(_target);
	}

}

void BoundingBox::CollisionEnd(BoundingBox* _target)
{
	if (owner == nullptr)
	{
		return;
	}

	if (owner && owner->IsActive() && _target && _target->IsActive())
	{
		owner->CollisionEnd(_target);
	}
}

void BoundingBox::Update(float deltaTime)
{
	Super::Update(deltaTime);
	if (owner)
	{
		SetPosition(owner->Position() + offset);
	}
	x_min = position.x - width / 2;
	y_min = position.y - height / 2;
	x_max = position.x + width / 2;
	y_max = position.y + height / 2;

}

void BoundingBox::Draw()
{
	Super::Draw();
	//Engine::Get().Draw(position, image, color, width, height);
}
