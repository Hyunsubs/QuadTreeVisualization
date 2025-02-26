#pragma once
#include "PreCompiledHeader/PreCompiledHeader.h"

class SoundObject;
class ENGINE_API SoundManager
{
public:
	SoundManager();
	~SoundManager();

	static void Init();
	inline static SoundManager& Get() { return *instance; }
	inline static FMOD::System& GetFMODSystem() { return *system; }


private:
	static SoundManager* instance;
	static FMOD::System* system;


};