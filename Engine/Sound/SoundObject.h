#pragma once

#include "PreCompiledHeader/PreCompiledHeader.h"

class ENGINE_API SoundObject
{
private:
	FMOD::Sound* m_Sound;	// Sound 버퍼
	std::list<FMOD::Channel*> m_listChannel; // Sound가 재생되고 있는 채널 리스트

public:
    // _iRoopCount : 0 (무한반복),  _fVolume : 0 ~ 1(Volume), _bOverlap : 소리 중첩 가능여부
    void LoadSound(std::string _path);
    int Play(int _iRoopCount, float _fVolume, bool _bOverlap);
    void RemoveChannel(FMOD::Channel* _pTargetChannel);
    void Stop();

    // 0 ~ 1
    void SetVolume(float _f, int _iChannelIdx);
    ~SoundObject();

};