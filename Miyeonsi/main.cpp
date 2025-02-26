#include <iostream>
#include <windows.h>
#include "Engine/Engine.h"
#include "Sound/SoundManager.h"
#include "Sound/SoundObject.h"
#include "Levels/TitleLevel.h"
#include "Levels/OpeningLevel.h"
#include "Levels/RBTDemoLevel.h"
using namespace std;

int cur_level = 0;

bool print_collision = false;
std::vector<Level*> levels;

int main()
{
	// �� üũ
	CheckMemoryLeak();

	// ���� ���� ����
	Engine engine;

	SoundManager::Get().Init();

	// ���� ���� ����
	SetConsoleTitleA("RBT Demo");
	RBTDemoLevel* rbtLevel = new RBTDemoLevel();
	levels.push_back(rbtLevel);
	engine.LoadLevel(levels[cur_level]);
	engine.SetPrintCollision(true);
	for (int i = 0; i < levels.size(); i++)
	{
		levels[i]->Init();
	}
	engine.Run();

	return 0;
}

