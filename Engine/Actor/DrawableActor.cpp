#include "PreCompiledHeader.h"
#include "DrawableActor.h"
#include "Engine/Engine.h"
#include "Util.h"

DrawableActor::DrawableActor(const char* image)
	: Actor()/*, image(image)*/
	, image(image)
{
	this->image.reserve(3000);
}

DrawableActor::DrawableActor(int a, std::string _filepath)
	: Actor()
{
	std::string file = Util::LoadFileContent(_filepath.c_str());
	image = file;
}


DrawableActor::~DrawableActor()
{
}

void DrawableActor::Update(float deltaTime)
{
	width = (int)image.find('\n');
	height = (int)image.size() / width;
}

void DrawableActor::Draw()
{
	Engine::Get().Draw(position, image, color, width, height, reverse);
}


void DrawableActor::SetPosition(const Vector2& newPosition)
{
	// 위치를 새로 옮기기.
	Super::SetPosition(newPosition);
}

bool DrawableActor::Intersect(const DrawableActor& other)
{
	// AABB(Axis Aligned Bounding Box).

	// 내 x좌표 최소/최대.
	int min = position.x;
	int max = position.x + width;

	// 다른 액터의 x좌표 최소/최대.
	int otherMin = other.position.x;
	int otherMax = other.position.x + other.width;

	// 다른 액터의 왼쪽 끝 위치가 내 오른쪽 끝 위치를 벗어나면 충돌 안함.
	if (otherMin > max)
	{
		return false;
	}

	// 다른 액터의 오른쪽 끝 위치가 내 왼쪽 끝 위치보다 작으면 충돌 안함.
	if (otherMax < min)
	{
		return false;
	}

	// 위의 두 경우가 아니라면 (x좌표는 서로 겹침), y위치 비교.
	return position.y == other.position.y;
}

void DrawableActor::OnCollisionEnter(BoundingBox* _target)
{
}

void DrawableActor::OnCollision(BoundingBox* _target)
{
}

void DrawableActor::CollisionEnd(BoundingBox* _target)
{
}
