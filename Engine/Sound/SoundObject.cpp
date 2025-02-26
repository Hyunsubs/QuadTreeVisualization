#include "PreCompiledHeader.h"
#include "SoundObject.h"
#include "SoundManager.h"



FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
    , FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
    , void* commanddata1, void* commanddata2);


SoundObject::~SoundObject()
{
    if (nullptr != m_Sound)
    {
        FMOD_RESULT result = m_Sound->release();
        m_Sound = nullptr;
    }
}


void SoundObject::LoadSound(std::string _path)
{
	if (FMOD_OK != SoundManager::Get().GetFMODSystem().createSound(_path.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_Sound))
	{
		assert(nullptr);
	}
}

int SoundObject::Play(int _iRoopCount, float _fVolume, bool _bOverlap)
{
	if (_iRoopCount <= -1)
	{
		assert(nullptr);
	}

	// 재생되고 있는 채널이 있는데, 중복재생을 허용하지 않았다 -> 재생 안함
	if (!_bOverlap && !m_listChannel.empty())
	{
		return E_FAIL;
	}

	_iRoopCount -= 1;


	FMOD::Channel* pChannel = nullptr;
	SoundManager::Get().GetFMODSystem().playSound(m_Sound, nullptr, false, &pChannel);

	// 재생 실패
	if (nullptr == pChannel)
		return E_FAIL;

	pChannel->setVolume(_fVolume);

	pChannel->setCallback(&CHANNEL_CALLBACK);
	pChannel->setUserData(this);

	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(_iRoopCount);

	m_listChannel.push_back(pChannel);

	int iIdx = -1;
	pChannel->getIndex(&iIdx);

	return iIdx;
}

void SoundObject::RemoveChannel(FMOD::Channel* _pTargetChannel)
{
	std::list<FMOD::Channel*>::iterator iter = m_listChannel.begin();
	for (; iter != m_listChannel.end(); ++iter)
	{
		if (*iter == _pTargetChannel)
		{
			m_listChannel.erase(iter);
			return;
		}
	}
}

void SoundObject::Stop()
{
	std::list<FMOD::Channel*>::iterator iter;

	while (!m_listChannel.empty())
	{
		iter = m_listChannel.begin();
		(*iter)->stop();
	}
}

void SoundObject::SetVolume(float _f, int _iChannelIdx)
{
	std::list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

	int iIdx = -1;
	for (; iter != m_listChannel.end(); ++iter)
	{
		(*iter)->getIndex(&iIdx);
		if (_iChannelIdx == iIdx)
		{
			(*iter)->setVolume(_f);
			return;
		}
	}
}


// =========
// Call Back
// =========
FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2)
{
	FMOD::Channel* cppchannel = (FMOD::Channel*)channelcontrol;
	SoundObject* pSound = nullptr;

	switch (controltype)
	{
	case FMOD_CHANNELCONTROL_CALLBACK_END:
	{
		cppchannel->getUserData((void**)&pSound);
		pSound->RemoveChannel(cppchannel);
	}
	break;
	}

	return FMOD_OK;
}
