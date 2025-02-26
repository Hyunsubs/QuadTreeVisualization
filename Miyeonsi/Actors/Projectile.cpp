#include "Projectile.h"
#include "Actor/AnimatedActor.h"
#include "Engine/Engine.h"
#include "Level/Level.h"
#include "Actor/BoundingBox.h"
#include "Sound/SoundObject.h"

Projectile::Projectile(bool _left, Vector2& position, PLAYER_STATE state)
	: Super()
{
	left = _left;
	color = Color::Blue;

	reverse = left;
	
	if (reverse)
		this->SetPosition(Vector2(position.x - 17, position.y));
	else
		this->SetPosition(Vector2(position.x + 17, position.y));

	AddAnimation("Assets\\imgs\\RyuAnimComplete\\Projectile\\images\\", 0.16f);

	box = new BoundingBox();
	box->SetOwner(this);
	box->SetSize(10, 10);
	box->SetOffset(0, 5);
	if (state == PLAYER_STATE::FIRST)
		box->SetLayerType(LAYER::RYUPROJECTILE);
	else
		box->SetLayerType(LAYER::KENPROJECTILE);

	Level* curLevel = Engine::Get().GetCurrentLevel();
	curLevel->AddActor(box);

}

Projectile::~Projectile()
{
}

void Projectile::Update(float deltaTime)
{
	Super::Update(deltaTime);
	Vector2 CurPos = position;

	if(!IsPlaying())
		PlayAnimation(0, true);
	
	if (CurPos.x < 0 + width / 2 || CurPos.x > 450)
	{
		box->Destroy();
		Destroy();
		return;
	}


	if (!left)
		CurPos.x += (int)ceilf(50.f * deltaTime);
	else
		CurPos.x -= (int)ceilf(50.f * deltaTime);

	SetPosition(CurPos);
}

void Projectile::Draw()
{
	Super::Draw();
}

void Projectile::OnCollisionEnter(BoundingBox* _target)
{
	BoundingBox* targetBox = (BoundingBox*)_target;
	Player* opponent = nullptr;
	if (targetBox->GetLayerType() == LAYER::KENBODY)
	{
		if (box->GetLayerType() == LAYER::RYUPROJECTILE)
		{
			opponent = (Player*)targetBox->GetOwner();
			if (opponent->IsDead())
				return;
			if (!opponent->IsGuard())
			{
				opponent->SetHP(opponent->GetHP() - 30);
				opponent->SetStunnedTime(0.5f);
				opponent->PlayPlayerSound(PlayerSound::STRONGHIT);
				opponent->ResetTimer();
				opponent->PlayAnimation((int)ANIM_LIST::STUMBLE, false);
			}

			else
			{
				opponent->SetStunnedTime(0.3f);
				opponent->PlayPlayerSound(PlayerSound::BLOCK);
				opponent->ResetTimer();
				opponent->PlayAnimation((int)ANIM_LIST::GUARD, false);
			}
			box->Destroy();
			Destroy();
			return;
		}

	}
	else if (targetBox->GetLayerType() == LAYER::RYUBODY)
	{
		if (box->GetLayerType() == LAYER::KENPROJECTILE)
		{
			opponent = (Player*)targetBox->GetOwner();
			if (opponent->IsDead())
				return;
			if (!opponent->IsGuard())
			{
				opponent->SetHP(opponent->GetHP() - 30);
				opponent->SetStunnedTime(0.5f);
				opponent->PlayPlayerSound(PlayerSound::STRONGHIT);
				opponent->ResetTimer();
				opponent->PlayAnimation((int)ANIM_LIST::STUMBLE, false);
			}

			else
			{
				opponent->SetStunnedTime(0.3f);
				opponent->PlayPlayerSound(PlayerSound::BLOCK);
				opponent->ResetTimer();
				opponent->PlayAnimation((int)ANIM_LIST::GUARD, false);
			}
			box->Destroy();
			Destroy();
			return;
		}
	}

	if ((box->GetLayerType() == LAYER::KENPROJECTILE && targetBox->GetLayerType() == LAYER::RYUPROJECTILE) ||
		(box->GetLayerType() == LAYER::RYUPROJECTILE && targetBox->GetLayerType() == LAYER::KENPROJECTILE))
	{
		targetBox->GetOwner()->Destroy();
		targetBox->Destroy();
		box->Destroy();
		Destroy();
		return;
	}
}

void Projectile::OnCollision(BoundingBox* _target)
{

}

void Projectile::CollisionEnd(BoundingBox* _target)
{
}
