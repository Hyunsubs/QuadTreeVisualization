#include "OpeningLevel.h"
#include "Sound/SoundObject.h"
#include "Engine/Engine.h"
#include "Engine/Timer.h"
#include "Actor/DrawableActor.h"
#include "Util.h"

extern int cur_level;
extern std::vector<Level*> levels;

OpeningLevel::OpeningLevel()
{
}

OpeningLevel::~OpeningLevel()
{
}

void OpeningLevel::Init()
{
	openingSound = new SoundObject();
	openingSound->LoadSound("Assets\\sound\\Opening.flac");
	openingTimer = new Timer(3.f);
	flasher = new Timer(0.5f);

	title = new DrawableActor(Util::LoadFileContent("Assets\\imgs\\logo.txt").c_str());
	title->SetPosition(Vector2(200, 0));
	pressStart = new DrawableActor(Util::LoadFileContent("Assets\\imgs\\pressStart.txt").c_str());
	pressStart->SetPosition(Vector2(0, -200));

	AddActor(title);
	AddActor(pressStart);
}

void OpeningLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);
	openingTimer->Update(deltaTime);

	if (openingTimer->IsTimeOut())
	{
		flasher->Update(deltaTime);
	}

	if (flasher->IsTimeOut())
	{
		if (pressStart->Position().y < 0)
			pressStart->SetPosition(Vector2(200, 100));
		else
			pressStart->SetPosition(Vector2(200, -200));

		flasher->Reset();
	}

	if (!started)
	{
		started = true;
		openingSound->Play(0, 0.7f, false);
	}

	if (title->Position().y < 60)
	{
		Vector2 pos = title->Position();
		pos.y += (int)ceilf(10.f * deltaTime);
		title->SetPosition(pos);
	}

	if (Engine::Get().GetKeyDown(VK_RETURN) && openingTimer->IsTimeOut())
	{
		openingSound->Stop();
		cur_level++;
		Engine::Get().LoadLevel(levels[cur_level]);
	}

}

void OpeningLevel::Draw()
{
	Super::Draw();
}
