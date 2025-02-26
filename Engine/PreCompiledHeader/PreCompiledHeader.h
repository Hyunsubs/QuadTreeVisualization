#pragma once

#pragma warning (disable : 4251)

// 미리 컴파일된 헤더.
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <list>

// fmod 스레드 관리용
#include <chrono>
#include <thread>

#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#include "Math/Vector2.h"


#ifdef _DEBUG
#pragma comment(lib, "fmodL64_vc.lib")
#else
#pragma comment(lib, "fmod64_vc.lib")
#endif

