#include "PreCompiledHeader.h"
#include "SoundManager.h"


SoundManager* SoundManager::instance = nullptr;
FMOD::System* SoundManager::system = nullptr;


SoundManager::SoundManager()
{
	instance = this;
}

SoundManager::~SoundManager()
{
	SafeDelete(instance);
	system->release();
}

void SoundManager::Init()
{
	FMOD::System_Create(&system);
	assert(&system);
	system->init(32, FMOD_DEFAULT, nullptr);

}
