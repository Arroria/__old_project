#pragma once
struct WndInfo
{
	HWND hWnd;
	DWORD wndStyle;
	std::wstring name;

	//int xPos;
	//int yPos;
	unsigned int xSize;
	unsigned int ySize;

	WndInfo();
};

class ProcessManager
{
private:
	HINSTANCE m_hInstance;
	WndInfo* m_wndInfo;

public:
	HRESULT CreateWnd(const std::wstring& wndName, const DWORD& wndStyle, LRESULT(CALLBACK *wndProc)(HWND, UINT, WPARAM, LPARAM));
	void WndMove(const int& xPos, const int& yPos);
	void WndResize(const unsigned int& xSize, const unsigned int& ySize);

	WndInfo* GetWndInfo() { return m_wndInfo; }


private:
	LPDIRECT3DDEVICE9 m_d3dxDevice;

public:
	HRESULT CreateDevice(const bool& isWindow);

	LPDIRECT3DDEVICE9 GetDevice() { return m_d3dxDevice; }


private:
	HRESULT(*m_initialize)();
	void(*m_update)(double);
	HRESULT(*m_render)();
	void(*m_release)();

	size_t m_FPSLimite;
	double m_updateDeltaTime;
	
public:
	WPARAM Loop();

	void SetFunction(HRESULT(*initialize)(), void(*update)(double), HRESULT(*render)(), void(*release)()) { m_initialize = initialize; m_update = update; m_render = render; m_release = release; }
	void SetFPSLimite(const size_t& fps) { m_FPSLimite = fps; }
	double GetDeltaTime() { return m_updateDeltaTime; }


public:
	ProcessManager(const HINSTANCE& hInstance);
	~ProcessManager();
};

