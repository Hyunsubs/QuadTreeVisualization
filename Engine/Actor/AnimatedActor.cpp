#include "PreCompiledHeader.h"
#include "AnimatedActor.h"
#include "Engine/Timer.h"
#include <strsafe.h>
#include <tchar.h>
#include "Util.h"

AnimatedActor::AnimatedActor()
	: Super("None")
	, anim_timer(new Timer(0.f))
{
}

AnimatedActor::~AnimatedActor()
{
	SafeDelete(anim_timer);
}


int AnimatedActor::AddAnimation(std::string _filepath, float anim_time)
{
	int file_number = 0;
	std::vector<std::string> container;
	std::vector<std::string> filenames;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	std::string filepath = _filepath + "*.txt";
	hFind = FindFirstFileA(filepath.c_str(), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("FindFirstFile failed (%d)\n"), GetLastError());
		return -1;
	}

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) // 파일만 검색
		{
			filenames.push_back(FindFileData.cFileName); // 파일 이름 저장
			file_number++;
		}
	} while (FindNextFileA(hFind, &FindFileData) != 0);

	FindClose(hFind);

	
	// 파일 이름에서 숫자 제거
	filenames[0].erase(remove_if(filenames[0].begin(), filenames[0].end(), ::isdigit), filenames[0].end());
	int fileEnd = (int)filenames[0].find('.');
	
	
	// 파일 내용을 로드
	for (int i = 0; i < file_number; i++)
	{
		std::string temp = filenames[0];
		std::string file = temp.insert(fileEnd, std::to_string(i));
		std::string final_path = _filepath + file;
		container.push_back(Util::LoadFileContent(final_path.c_str()));
	}


	imageList.push_back(container);
	this->anim_time.push_back(anim_time);

	return 0;
}

void AnimatedActor::Update(float deltaTime)
{
	Super::Update(deltaTime);
	anim_timer->Update(deltaTime);

	if (is_playing)
	{
		if (current_frame >= imageList[current_anim_idx].size())
		{
			if (repeat)
			{
				current_frame = 0;
			}

			else
			{
				current_frame = 0;
				is_playing = false;
				return;
			}
		}

		this->image = imageList[current_anim_idx][current_frame].c_str();

		if (anim_timer->IsTimeOut())
		{
			anim_timer->Reset();
			current_frame++;
		}

	}
}

void AnimatedActor::Draw()
{
	Super::Draw();
}


void AnimatedActor::PlayAnimation(int idx, bool repeat)
{
	if (idx < imageList.size())
	{
		current_frame = 0;
		current_anim_idx = idx;
		this->repeat = repeat;
		this->is_playing = true;
		anim_timer->SetTime(anim_time[idx]);
	}
}

void AnimatedActor::OnCollisionEnter(BoundingBox* _target)
{
}

void AnimatedActor::OnCollision(BoundingBox* _target)
{
}

void AnimatedActor::CollisionEnd(BoundingBox* _target)
{
}
