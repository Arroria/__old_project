#include "DXUT.h"
#include "SoundManager.h"



SoundManager::SoundManager()
	: m_dxSndMgr(nullptr)
{
	m_dxSndMgr = new CSoundManager();
	m_dxSndMgr->Initialize(DXUTGetHWND(), DSSCL_PRIORITY);
}
SoundManager::~SoundManager()
{
}



Sound * SoundManager::LoadSound(LPWSTR path, const std::wstring & key, const UINT & channel)
{
	auto& iter = m_snd.find(key);
	if (iter != m_snd.end())
		return iter->second;

	Sound* snd = new Sound();
	snd->m_dxSnd.resize(channel);
	for (UINT i = 0; i < channel; i++)
	{
		if (FAILED( m_dxSndMgr->Create(&(snd->m_dxSnd[i].m_dxSnd), path) ))
		{
			SAFE_DELETE(snd);
			return nullptr;
		}
	}
	m_snd.insert(std::make_pair(key, snd));
	return snd;
}

Sound * SoundManager::GetSound(const std::wstring & key)
{
	auto& iter = m_snd.find(key);
	if (iter == m_snd.end())
		return nullptr;
	return iter->second;
}





Sound::Sound()
{
}
Sound::~Sound()
{
}



Channel * Sound::Play()
{
	for (auto& iter : m_dxSnd)
	{
		if (!iter.IsPlaying())
		{
			iter.Play();
			return &iter;
		}
	}
	return nullptr;
}





Channel::Channel()
	: m_dxSnd(nullptr)
{
}
Channel::~Channel()
{
}



void Channel::Play()
{
	if (m_dxSnd)
		m_dxSnd->Play();
}

void Channel::Stop()
{
	if (m_dxSnd)
		m_dxSnd->Stop();
}

bool Channel::IsPlaying()
{
	return m_dxSnd ?
		m_dxSnd->IsSoundPlaying() :
		false;
}
