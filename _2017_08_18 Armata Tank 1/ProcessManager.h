#pragma once
namespace AF
{
	struct WindowInfo
	{
		HWND			hWnd;

		std::wstring	name;
		DWORD			style;

		size_t			wndSizeX;
		size_t			wndSizeY;
		size_t			clientAreaSizeX;
		size_t			clientAreaSizeY;
		int				posX;
		int				posY;

		bool			isWindow;
		bool			isActive;
	};



	class ProcessManager final
	{
	private:
		HINSTANCE	m_hInstance;
		int			m_cmdShow;

	public:
		const HINSTANCE&	GetHINSTANCE() { return m_hInstance; }



	private:
		WindowInfo	m_windowInfo;
		LRESULT(*m_wndProc)(const HWND&, const UINT&, const WPARAM&, const LPARAM&);
		static	LRESULT CALLBACK    MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT WndProc(const HWND& hWnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam);

	public:
		HRESULT	CreateWnd	(const std::wstring& wndName, const DWORD& wndStyle,
							const size_t& wndSizeX, const size_t& wndSizeY, const bool& isUseMenu = false);
		HRESULT	CreateWnd	(const std::wstring& wndName, const DWORD& wndStyle,
							const size_t& wndSizeX, const size_t& wndSizeY, const int& wndPosX, const int& wndPosY, const bool& isUseMenu = false);
		void	SetWndProc	(LRESULT(*wndProc)(const HWND&, const UINT&, const WPARAM&, const LPARAM&))	{ m_wndProc = wndProc; }

		const	WindowInfo&	GetWindowInfo()	{ return m_windowInfo; }
		


	private:
		LPDIRECT3DDEVICE9	m_d3dxDevice;

	public:
		HRESULT	CreateDevice();
		void	ReleaseDevice() { SAFE_RELEASE( m_d3dxDevice ); }

		LPDIRECT3DDEVICE9	GetDevice()	{ return m_d3dxDevice; }



	private:
		HRESULT(*m_initialize)	();
		HRESULT(*m_update)		();
		HRESULT(*m_render)		();
		HRESULT(*m_release)		();

	public:
		void	SetCallbackFunction(HRESULT(*initialize)(), HRESULT(*update)(), HRESULT(*render)(), HRESULT(*release)());
		int		ProcessRun();


	private:
		ProcessManager();
	public:
		ProcessManager(const HINSTANCE& hInstance, const int& cmdShow);
		~ProcessManager();
	};


	extern ProcessManager*	g_processManager;
}

