#include "PreCompiledHeader.h"

#include "Engine.h"
#include <Windows.h>
#include <iostream>

#include "Level/Level.h"
#include "Actor/Actor.h"
#include "Sound/SoundManager.h"
#include "Sound/SoundObject.h"

#include <time.h>

#include "Render/ScreenBuffer.h"
#include "Actor/BoundingBox.h"
#include "Collision/CollisionManager.h"

// 콘솔 창 메시지 콜백 함수.
BOOL WINAPI MessageProcessor(DWORD message)
{
	switch (message)
	{
	case CTRL_CLOSE_EVENT:
		Engine::Get().QuitGame();
		return true;

	default:
		return false;
	}
}



// 스태틱 변수 초기화.
Engine* Engine::instance = nullptr;
bool Engine::print_collision = false;

Engine::Engine()
	: quit(false), mainLevel(nullptr), screenSize(500, 250)
{
	// 랜덤 시드 설정.
	srand((unsigned int)time(nullptr));

	// 싱글톤 객체 설정.
	instance = this;

	// 기본 타겟 프레임 속도 설정.
	SetTargetFrameRate(120.0f);
	SetConsoleFontSize(5);
	system("mode con lines=250 cols=600");

	// 화면 버퍼 초기화.
	// 
	// 1. 버퍼 크기 할당.
	imageBuffer = new CHAR_INFO[(screenSize.x + 1) * screenSize.y + 1];

	// 버퍼 초기화.
	ClearImageBuffer();

	// 두 개의 버퍼 생성 (버퍼를 번갈아 사용하기 위해-더블 버퍼링).
	COORD size = { (short)screenSize.x, (short)screenSize.y };
	renderTargets[0] = new ScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE), size);
	renderTargets[1] = new ScreenBuffer(size);


	// 스왑 버퍼.
	Present();

	// 콘솔 창 이벤트 콜백 함수 등록.
	SetConsoleCtrlHandler(MessageProcessor, true);


	// 마우스/윈도우 이벤트 활성화.
	HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	int flag = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(inputHandle, flag);

	// std::cin/std::cout 연결 끊기.
	std::ios::sync_with_stdio(false);

	collisionManager = new CollisionManager();
}

Engine::~Engine()
{
	// 메인 레벨 메모리 해제.
	if (mainLevel != nullptr)
	{
		delete mainLevel;
	}

	// 클리어 버퍼 삭제.
	delete[] imageBuffer;

	// 화면 버퍼 삭제.
	delete renderTargets[0];
	delete renderTargets[1];
}

void Engine::Run()
{
	// 시작 타임 스탬프 저장.
	// timeGetTime 함수는 밀리세컨드(1/1000초) 단위.
	//unsigned long currentTime = timeGetTime();
	//unsigned long previousTime = 0;

	// CPU 시계 사용.
	// 시스템 시계 -> 고해상도 카운터. (10000000).
	// 메인보드에 시계가 있음.
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	//std::cout << "Frequency: " << frequency.QuadPart << "\n";

	// 시작 시간 및 이전 시간을 위한 변수.
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	int64_t currentTime = time.QuadPart;
	int64_t previousTime = currentTime;

	//// 프레임 제한.
	// float targetFrameRate = 60.f;

	//// 한 프레임 시간 계산.
	//float targetOneFrameTime = 1.0f / targetFrameRate;

	collisionManager->SetCollision(LAYER::GROUND, LAYER::RYUBODY);
	collisionManager->SetCollision(LAYER::GROUND, LAYER::KENBODY);
	collisionManager->SetCollision(LAYER::RYU, LAYER::KENBODY);
	collisionManager->SetCollision(LAYER::KEN, LAYER::RYUBODY);
	collisionManager->SetCollision(LAYER::RYU, LAYER::KEN);
	collisionManager->SetCollision(LAYER::KENPROJECTILE, LAYER::RYUBODY);
	collisionManager->SetCollision(LAYER::RYUPROJECTILE, LAYER::KENBODY);
	collisionManager->SetCollision(LAYER::RYUPROJECTILE, LAYER::KENPROJECTILE);

	// Game-Loop.
	while (true)
	{
		// 종료 조건.
		if (quit)
		{
			break;
		}

		// 현재 프레임 시간 저장.
		//time = timeGetTime();
		QueryPerformanceCounter(&time);
		currentTime = time.QuadPart;

		SoundManager::Get().GetFMODSystem().update();

		// 프레임 시간 계산.
		float deltaTime = static_cast<float>(currentTime - previousTime) /
			static_cast<float>(frequency.QuadPart);

		// 한 프레임 시간 계산.
		float targetOneFrameTime = 1.0f / targetFrameRate;

		// 프레임 확인.
		if (deltaTime >= targetOneFrameTime)
		{
			// 입력 처리 (현재 키의 눌림 상태 확인).
			ProcessInput();

			// 업데이트 가능한 상태에서만 프레임 업데이트 처리.
			if (shouldUpdate)
			{
				Update(deltaTime);
				collisionManager->Update();
				Draw();
			}

			// 키 상태 저장.
			SavePreviouseKeyStates();

			// 이전 프레임 시간 저장.
			previousTime = currentTime;

			// 액터 정리 (삭제 요청된 액터들 정리).
			if (mainLevel)
			{
				//mainLevel->DestroyActor();

				collisionManager->SetCurrentLevel(mainLevel);
				mainLevel->ProcessAddedAndDestroyedActor();

			}

			// 프레임 활성화.
			shouldUpdate = true;
		}

		//Sleep(1);
	}
}

void Engine::LoadLevel(Level* newLevel)
{
	// 기존 레벨이 있다면 삭제 후 교체.

	// 메인 레벨 설정.
	mainLevel = newLevel;
}

void Engine::AddActor(Actor* newActor)
{
	// 예외 처리.
	if (mainLevel == nullptr)
	{
		return;
	}

	// 레벨에 액터 추가.
	shouldUpdate = false;
	mainLevel->AddActor(newActor);
}

void Engine::DestroyActor(Actor* targetActor)
{
	// 예외 처리.
	if (mainLevel == nullptr)
	{
		return;
	}

	// 레벨에 액터 추가.
	shouldUpdate = false;
	targetActor->Destroy();
}

void Engine::SetCursorType(CursorType cursorType)
{
	// 1. 커서 속성 구조체 설정.
	CONSOLE_CURSOR_INFO info = { };

	// 타입 별로 구조체 값 설정.
	switch (cursorType)
	{
	case CursorType::NoCursor:
		info.dwSize = 1;
		info.bVisible = FALSE;
		break;

	case CursorType::SolidCursor:
		info.dwSize = 100;
		info.bVisible = TRUE;
		break;

	case CursorType::NormalCursor:
		info.dwSize = 20;
		info.bVisible = TRUE;
		break;

	}

	// 2. 설정.
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void Engine::SetCursorPosition(const Vector2& position)
{
	SetCursorPosition(position.x, position.y);
}

void Engine::SetCursorPosition(int x, int y)
{
	static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { static_cast<short>(x), static_cast<short>(y) };
	SetConsoleCursorPosition(handle, coord);
}

void Engine::SetTargetFrameRate(float targetFrameRate)
{
	this->targetFrameRate = targetFrameRate;
	targetOneFrameTime = 1.0f / targetFrameRate;
}

bool Engine::GetKey(int key)
{
	return keyState[key].isKeyDown;
}

bool Engine::GetKeyDown(int key)
{
	return keyState[key].isKeyDown && !keyState[key].wasKeyDown;
}

bool Engine::GetKeyUp(int key)
{
	return !keyState[key].isKeyDown && keyState[key].wasKeyDown;
}

void Engine::QuitGame()
{
	// 종료 플래그 설정.
	quit = true;
}

Engine& Engine::Get()
{
	// 싱글톤 객체 반환.
	return *instance;
}

void Engine::ProcessInput()
{
	static HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD record;
	DWORD events;
	if (PeekConsoleInput(inputHandle, &record, 1, &events) && events > 0)
	{
		if (ReadConsoleInput(inputHandle, &record, 1, &events))
		{
			switch (record.EventType)
			{
			case KEY_EVENT:
			{
				// 키 눌림 상태 업데이트.
				if (record.Event.KeyEvent.bKeyDown)
				{
					keyState[record.Event.KeyEvent.wVirtualKeyCode].isKeyDown = true;
				}
				// 키 눌림 해제 상태 업데이트.
				else
				{
					keyState[record.Event.KeyEvent.wVirtualKeyCode].isKeyDown = false;
				}
			}
			break;

			case MOUSE_EVENT:
			{
				// 마우스 커서 위치 업데이트.
				mousePosition.x = record.Event.MouseEvent.dwMousePosition.X;
				mousePosition.y = record.Event.MouseEvent.dwMousePosition.Y;

				// 마우스 왼쪽 버튼 클릭 상태 업데이트.
				keyState[VK_LBUTTON].isKeyDown
					= (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;

				// 마우스 오른쪽 버튼 클릭 상태 업데이트.
				keyState[VK_RBUTTON].isKeyDown
					= (record.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0;
			}
			break;

			//		//case WINDOW_BUFFER_SIZE_EVENT:
			//{
			//	char buffer[100];
			//	sprintf_s(buffer, 100, "(%d,%d)", 
			//		record.Event.WindowBufferSizeEvent.dwSize.X, record.Event.WindowBufferSizeEvent.dwSize.Y
			//	);

			//	MessageBoxA(nullptr, buffer, "Test", MB_OK);
			//} break;
			}
		}
	}

	//// 키 입력 테스트.
	//for (int ix = 0; ix < 255; ++ix)
	//{
	//	keyState[ix].isKeyDown = (GetAsyncKeyState(ix) & 0x8000) ? true : false;
	//}
}

void Engine::Update(float deltaTime)
{
	// 레벨 업데이트.
	if (mainLevel != nullptr)
	{
		mainLevel->Update(deltaTime);
	}
}

void Engine::Clear()
{
	ClearImageBuffer();
	GetRenderer()->Clear();
}

void Engine::Draw()
{
	// 화면 지우기.
	Clear();

	// 레벨 그리기.
	if (mainLevel != nullptr)
	{
		mainLevel->Draw();
	}

	// 백버퍼에 데이터 쓰기.
	GetRenderer()->Draw(imageBuffer);

	if (mainLevel->GetAfterDraw())
	{
		mainLevel->AfterDraw();
	}
	// 프론트<->백 버퍼 교환.
	Present();
}



void Engine::Draw(const Vector2& position, std::string& image, Color color, int _width, int _height, bool _rev)
{
	int width;
	int height;

	if (_width == 0 || _height == 0)
	{
		width = (int)image.find('\n');
		height = (int)image.size() / width;
	}

	else
	{
		width = _width;
		height = _height;
	}

	Vector2 LeftTop = Vector2(position.x - (width / 2), position.y - (height / 2));

	if (!_rev)
	{
		int i = 0;
		for (int ix = LeftTop.y; ix < LeftTop.y + height - 2; ix++)
		{
			for (int jx = LeftTop.x; jx <= LeftTop.x + width; jx++)
			{
				if (ix < 0 || ix > Engine::Get().ScreenSize().y || jx < 0 || jx > Engine::Get().ScreenSize().y)
				{
					i++;
					continue;
				}

				if (i > image.size())
				{
					continue;
				}

				if (image[i] == '\n')
				{
					i++;
					//jx--; // 다음 줄로 넘어가도록 조정
					continue;
				}
				// 화면 범위를 벗어나는 경우를 처리
				if (ix < 0 || jx < 0 || ix >= ScreenSize().y || jx >= ScreenSize().x)
				{
					i++;
					continue;
				}

				int index = ix * ScreenSize().x + jx;
				if (i < image.size())
				{
					if (image[i] != ' ')
					{
						imageBuffer[index].Char.AsciiChar = image[i];
						imageBuffer[index].Attributes = (unsigned long)color;
					}
					i++;
				}
			}
		}
	}

	else
	{
		int i = 0;
		for (int ix = LeftTop.y; ix < LeftTop.y + height - 2; ix++)
		{
			for (int jx = LeftTop.x + width; jx >= LeftTop.x; jx--)
			{
				if (image[i] == '\n')
				{
					i++;
					//jx--; // 다음 줄로 넘어가도록 조정
					continue;
				}
				// 화면 범위를 벗어나는 경우를 처리
				if (ix < 0 || jx < 0 || ix >= ScreenSize().y || jx >= ScreenSize().x)
				{
					i++;
					continue;
				}

				int index = ix * ScreenSize().x + jx;
				if (i < image.size())
				{
					if (image[i] != ' ')
					{
						imageBuffer[index].Char.AsciiChar = image[i];
						imageBuffer[index].Attributes = (unsigned long)color;
					}
					i++;
				}
			}
		}
	}

}



void Engine::Present()
{
	// Swap Buffer.
	SetConsoleActiveScreenBuffer(GetRenderer()->buffer);
	currentRenderTargetIndex = 1 - currentRenderTargetIndex;
}

void Engine::SavePreviouseKeyStates()
{
	for (int ix = 0; ix < 255; ++ix)
	{
		keyState[ix].wasKeyDown = keyState[ix].isKeyDown;
	}
}

void Engine::ClearImageBuffer()
{
	// 버퍼 덮어쓰기.
	for (int y = 0; y < screenSize.y; ++y)
	{
		// 버퍼 덮어쓰기.
		for (int x = 0; x < screenSize.x + 1; ++x)
		{
			auto& buffer = imageBuffer[(y * (screenSize.x + 1)) + x];
			buffer.Char.AsciiChar = ' ';
			buffer.Attributes = 0;
		}

		// 각 줄 끝에 개행 문자 추가.
		auto& buffer = imageBuffer[(y * (screenSize.x + 1)) + screenSize.x];
		buffer.Char.AsciiChar = '\n';
		buffer.Attributes = 0;
	}

	// 마지막에 널 문자 추가.
	auto& buffer = imageBuffer[(screenSize.x + 1) * screenSize.y];
	buffer.Char.AsciiChar = '\0';
	buffer.Attributes = 0;
}


void Engine::ResizeConsole(int width, int height)
{
	// 콘솔 핸들 가져오기
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// 콘솔 화면 버퍼 크기 설정
	COORD bufferSize;
	bufferSize.X = width;
	bufferSize.Y = height;
	SetConsoleScreenBufferSize(hConsole, bufferSize);
	// 콘솔 창 크기 설정 
	SMALL_RECT windowSize;
	windowSize.Left = 0;
	windowSize.Top = 0;
	windowSize.Right = width - 1;
	windowSize.Bottom = height - 1;
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}


void Engine::SetConsoleFontSize(int size)
{
	// 콘솔 핸들 가져오기 
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// 폰트 정보를 명확히 초기화 
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };
	cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	// 너비는 0으로 설정하여 자동 결정 
	cfi.dwFontSize.Y = size; // 높이 (글꼴 크기)
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Courier");
	// 폰트 이름 설정 // 글꼴 설정 적용
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}

// 핸들에 맞게 폰트 사이즈 초기화
void Engine::SetConsoleFontSize(HANDLE& handle, int size)
{
	// 폰트 정보를 명확히 초기화 
	CONSOLE_FONT_INFOEX cfi = { sizeof(CONSOLE_FONT_INFOEX) };
	cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	// 너비는 0으로 설정하여 자동 결정 
	cfi.dwFontSize.Y = size; // 높이 (글꼴 크기)
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	// 폰트 이름 설정 // 글꼴 설정 적용
	SetCurrentConsoleFontEx(handle, FALSE, & cfi);
}


