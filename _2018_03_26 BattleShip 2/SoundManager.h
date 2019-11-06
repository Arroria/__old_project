#pragma once
#include "Singleton.h"

class SoundManager;
class Channel
{
	friend SoundManager;
private:
	CSound* m_snd;

public:
	void Play();
	void Stop();
	bool IsPlaying();

public:
	Channel();
	~Channel();
};

class Sound
{
	friend SoundManager;
private:
	std::vector<Channel> m_channel;

public:
	Channel* Play();

public:
	Sound();
	~Sound();
};

class SoundManager :
	public Singleton<SoundManager>
{
private:
	CSoundManager* m_dxutSnd;
	std::map<std::wstring, Sound*> m_sndList;

public:
	Sound* Load(LPWSTR path, const std::wstring& key, const UINT& channel = 1);
	Channel* Play(const std::wstring& key);

public:
	SoundManager();
	~SoundManager();
};

