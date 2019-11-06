#include "DXUT.h"
#include "SoundManager.h"


SoundManager::SoundManager()
{
	m_dxutSnd = new CSoundManager;
	m_dxutSnd->Initialize(DXUTGetHWND(), DSSCL_PRIORITY);
}
SoundManager::~SoundManager()
{
	for (auto& snd : m_sndList)
		SAFE_DELETE(snd.second);
	m_sndList.clear();
}



Sound * SoundManager::Load(LPWSTR path, const std::wstring & key, const UINT & channel)
{
	{
		auto snd = m_sndList.find(key);
		if (snd != m_sndList.end())
			return snd->second;
	}

	Sound* snd = new Sound;
	snd->m_channel.resize(channel);
	for (UINT i = 0; i < channel; i++)
	{
		CSound * sound = nullptr;
		HRESULT hr = m_dxutSnd->Create(&sound, path);
		snd->m_channel[i].m_snd = sound;
	}
	
	m_sndList.insert(std::make_pair(key, snd));
	return snd;
}

Channel * SoundManager::Play(const std::wstring & key)
{
	auto snd = m_sndList.find(key);
	if (snd == m_sndList.end())
		return nullptr;

	return snd->second->Play();
}





Sound::Sound()
{
}
Sound::~Sound()
{
}



Channel * Sound::Play()
{
	for (auto& channel : m_channel)
	{
		if (channel.IsPlaying())
			continue;

		channel.Play();
		return &channel;
	}
	return nullptr;
}





Channel::Channel()
	: m_snd(nullptr)
{
}
Channel::~Channel()
{
}



void Channel::Play()
{
	m_snd->Play();
}

void Channel::Stop()
{
	m_snd->Stop();
}

bool Channel::IsPlaying()
{
	return m_snd->IsSoundPlaying();
}

