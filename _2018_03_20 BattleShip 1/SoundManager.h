#pragma once
#include "Singleton.h"

//#include "./DXUT/Optional/SDKsound.h"
//#include "./DXUT/Optional/SDKwavefile.h"
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/dsound.lib")	


class SoundManager;
class Channel
{
	friend SoundManager;
private:
	CSound* m_dxSnd;

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
	std::vector<Channel> m_dxSnd;

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
	CSoundManager* m_dxSndMgr;
	std::map<std::wstring, Sound*> m_snd;

public:
	Sound* LoadSound(LPWSTR path, const std::wstring& key, const UINT& channel);
	Sound* GetSound(const std::wstring& key);
	Channel* Play(const std::wstring& key) { return GetSound(key)->Play(); }

public:
	SoundManager();
	~SoundManager();
};

