#pragma once
class MainLoop
{
public:
	static	HRESULT	Initialize();
	static	void	Update();
	static	void	Render();
	static	void	Release();

public:
	MainLoop();
	~MainLoop();
};

