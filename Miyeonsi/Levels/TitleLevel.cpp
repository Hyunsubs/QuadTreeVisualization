#include "TitleLevel.h"
#include "Sound/SoundObject.h"
#include "Functions/func.h"
#include "Actor/DrawableActor.h"
#include "Actor/AnimatedActor.h"
#include "Actor/BoundingBox.h"
#include "Actors/Player.h"
#include "Actors/HealthBar.h"
#include "Engine/Engine.h"
#include "Util.h"


TitleLevel::TitleLevel()
{

}

TitleLevel::~TitleLevel()
{
	if (testSound)
	{
		delete testSound;
		testSound = nullptr;
	}
}


void TitleLevel::Init()
{
	DrawableActor* background = new DrawableActor(Util::LoadFileContent("Assets\\imgs\\RyuStage.txt").c_str());
	background->SetPosition(Vector2(250, 100));
	AddActor(background);

	// 땅 렌더링할 그라운딩 박스
	BoundingBox* ground = new BoundingBox();
	ground->SetPosition(Vector2(500, 200));
	ground->SetSize(1000, 10);
	ground->SetLayerType(LAYER::GROUND);
	ground->SetOffset(0, 40);
	ground->SetOwner(background);
	AddActor(ground);

	// 1P 2P 설정
	Player* first_player = new Player();
	first_player->SetPosition(Vector2(50, 120));
	first_player->PlayAnimation(0, false);
	first_player->SetPlayerState(PLAYER_STATE::FIRST);
	AddActor(first_player);
	players[0] = first_player;

	Player* second_player = new Player();
	second_player->SetPosition(Vector2(420, 120));
	second_player->PlayAnimation(0, false);
	second_player->SetReverse(true);
	second_player->SetPlayerState(PLAYER_STATE::SECOND);
	AddActor(second_player);
	players[1] = second_player;

	// 1P 바운딩 박스 설정
	BoundingBox* handBox = new BoundingBox();
	handBox->SetOwner(first_player);
	handBox->SetSize(7, 7);
	handBox->SetLayerType(LAYER::RYU);
	AddActor(handBox);
	first_player->SetAttackBox(handBox);

	BoundingBox* bodyBox = new BoundingBox();
	bodyBox->SetOwner(first_player);
	bodyBox->SetLayerType(LAYER::RYUBODY);
	bodyBox->SetSize(30, 30);
	AddActor(bodyBox);
	first_player->SetHitBox(bodyBox);


	// 2P 바운딩 박스 설정
	BoundingBox* rev_box = new BoundingBox();
	rev_box->SetOwner(second_player);
	rev_box->SetLayerType(LAYER::KENBODY);
	rev_box->SetSize(30, 30);
	second_player->SetHitBox(rev_box);
	AddActor(rev_box);

	BoundingBox* secondHandBox = new BoundingBox();
	secondHandBox->SetOwner(second_player);
	secondHandBox->SetSize(7, 7);
	secondHandBox->SetLayerType(LAYER::KEN);
	second_player->SetAttackBox(secondHandBox);
	AddActor(secondHandBox);


	// HP바 설정
	HealthBar* first_health = new HealthBar();
	first_health->SetOwner(first_player);
	first_health->SetPosition(Vector2(80, 30));
	AddActor(first_health);

	HealthBar* second_health = new HealthBar();
	second_health->SetOwner(second_player);
	second_health->SetPosition(Vector2(380, 30));
	AddActor(second_health);
}

void TitleLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (!is_sound)
	{
		is_sound = true;
		testSound = new SoundObject;
		testSound->LoadSound("Assets\\sound\\GuileStage.mp3");
		testSound->Play(0, 0.3f, true);
	}

	if (players[0]->Position().x > players[1]->Position().x)
	{
		players[0]->SetReverse(true);
		players[1]->SetReverse(false);
	}
	else
	{
		players[0]->SetReverse(false);
		players[1]->SetReverse(true);
	}


	if (Engine::Get().GetKeyDown(VK_TAB))
	{
		Engine::Get().SetPrintCollision(!(Engine::Get().GetPrintCollision()));
	}
}

void TitleLevel::Draw()
{
	Super::Draw();

}

