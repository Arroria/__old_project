#pragma once
#include "Singleton.h"

class SoundManager :
	public Singleton<SoundManager>
{
private:
	LPDIRECTSOUND8 m_lpds;
	std::vector<LPDIRECTSOUNDBUFFER> m_lpdsBuffer;

public:
	int Play(const std::wstring& filePath);

public:
	SoundManager();
	~SoundManager();
};

