#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	DirectSoundCreate8(NULL, &m_lpds, NULL);
	m_lpds->SetCooperativeLevel(g_processManager->GetWndInfo()->hWnd, DSSCL_NORMAL);

	m_lpdsBuffer.resize(50);
	for (auto& iter : m_lpdsBuffer)
		iter = nullptr;
}

SoundManager::~SoundManager()
{
	for (auto& iter : m_lpdsBuffer)
		SAFE_RELEASE(iter);
}

int SoundManager::Play(const std::wstring& filePath)
{
	//Find Buffer
	LPDIRECTSOUNDBUFFER* soundBuffer = nullptr;
	for (auto& iter : m_lpdsBuffer)
	{
		//Empty buffer
		if (!iter)
		{
			soundBuffer = &iter;
			break;
		}
		
		//Non work(stop) buffer
		DWORD status;
		iter->GetStatus(&status);
		if (!(status & DSBSTATUS_PLAYING))
		{
			iter->Release();
			soundBuffer = &iter;
			break;
		}
	}

	//If can not found buffer, create buffer.
	if (!soundBuffer)
	{
		m_lpdsBuffer.push_back(nullptr);
		soundBuffer = &m_lpdsBuffer.back();
	}




	//뭔가 읽음
	HMMIO hWAV = mmioOpenW(const_cast<LPWSTR>(filePath.data()), NULL, MMIO_READ | MMIO_ALLOCBUF);
	if (!hWAV)
		return 0;

	MMCKINFO Parent, Child;
	ZeroMemory(&Parent, sizeof(Parent));
	ZeroMemory(&Child, sizeof(Child));
	Parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	Child.ckid = mmioFOURCC('f', 'm', 't', ' ');

	UCHAR *WholeSndBuff, *PrimaryBuff = NULL, *SecondaryBuff = NULL;
	DWORD PrimaryLength = 0, SecondaryLength = 0;


	WAVEFORMATEX wfx;
	DSBUFFERDESC dsbdesc;


	//웨이브 파일의 헤더를 읽는 부분
	if (mmioDescend(hWAV, &Parent, NULL, MMIO_FINDRIFF) ||
		mmioDescend(hWAV, &Child, &Parent, 0) ||
		mmioRead(hWAV, (char *)&wfx, sizeof(wfx)) != sizeof(wfx) ||
		wfx.wFormatTag != WAVE_FORMAT_PCM ||
		mmioAscend(hWAV, &Child, 0))
	{
		mmioClose(hWAV, 0);
		return 0;
	}

	Child.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(hWAV, &Child, &Parent, MMIO_FINDCHUNK))
	{
		mmioClose(hWAV, 0);
		return 0;
	}

	//웨이브 파일에서 읽은 데이타로 웨이브 데이타를 읽어 드림
	WholeSndBuff = new UCHAR[Child.cksize];
	mmioRead(hWAV, (char *)WholeSndBuff, Child.cksize);
	mmioClose(hWAV, 0);

	/*
	memset(&wfx, 0, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 44100;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = (wfx.nChannels*wfx.wBitsPerSample)/8;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.cbSize = 0;
	*/

	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags = DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME |
		DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS | DSBCAPS_STICKYFOCUS | DSBCAPS_GETCURRENTPOSITION2;
	dsbdesc.dwBufferBytes = Child.cksize;
	dsbdesc.lpwfxFormat = &wfx;

	if (FAILED(m_lpds->CreateSoundBuffer(&dsbdesc, soundBuffer, NULL)))
	{
		delete[] WholeSndBuff;
		return 0;
	}

	(*soundBuffer)->Lock(0, Child.cksize, (LPVOID*)&PrimaryBuff, &PrimaryLength, (LPVOID*)&SecondaryBuff, &SecondaryLength, DSBLOCK_FROMWRITECURSOR);
	memcpy(PrimaryBuff, WholeSndBuff, PrimaryLength);
	memcpy(SecondaryBuff, (WholeSndBuff + PrimaryLength), SecondaryLength);
	(*soundBuffer)->Unlock(PrimaryBuff, PrimaryLength, SecondaryBuff, SecondaryLength);
	delete[] WholeSndBuff;

	(*soundBuffer)->Play(0, 0, 0);

	return 1;
}