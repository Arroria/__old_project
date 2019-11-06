#pragma once
class CFish;
class CMainLoop
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

public:
	CMainLoop();
	~CMainLoop();
};
