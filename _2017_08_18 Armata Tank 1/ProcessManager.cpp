#include "MainHeader.h"
#include "ProcessManager.h"
#include "Resource.h"


namespace AF
{
	ProcessManager*	g_processManager = nullptr;


	ProcessManager::ProcessManager(const HINSTANCE & hInstance, const int & cmdShow)
		: m_hInstance(hInstance)
		, m_cmdShow(cmdShow)
	{
	}
	ProcessManager::~ProcessManager()
	{
	}



	HRESULT ProcessManager::CreateWnd(const std::wstring & wndName, const DWORD & wndStyle, const size_t & wndSizeX, const size_t & wndSizeY, const bool & isUseMenu)
	{
		int wndPosX = (GetSystemMetrics(SM_CXSCREEN) - wndSizeX) * 0.5f;
		int wndPosY = (GetSystemMetrics(SM_CYSCREEN) - wndSizeY) * 0.5f;
		return CreateWnd(wndName, wndStyle, wndSizeX, wndSizeY, (wndPosX > 0 ? wndPosX : 0), (wndPosY > 0 ? wndPosY : 0), isUseMenu);
	}
	HRESULT ProcessManager::CreateWnd(const std::wstring & wndName, const DWORD & wndStyle, const size_t & wndSizeX, const size_t & wndSizeY, const int & wndPosX, const int & wndPosY, const bool & isUseMenu)
	{
		//RegisterWindow
		WNDCLASSEXW wndClassEx = {0};

		wndClassEx.cbSize			= sizeof(WNDCLASSEX);

		wndClassEx.style			= CS_HREDRAW | CS_VREDRAW;
		wndClassEx.lpfnWndProc		= AF::ProcessManager::MainWndProc;
		wndClassEx.cbClsExtra		= 0;
		wndClassEx.cbWndExtra		= 0;
		wndClassEx.hInstance		= m_hInstance;
		wndClassEx.hIcon			= LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_WORLDWART_01));
		wndClassEx.hCursor			= LoadCursor(nullptr, IDC_ARROW);
		wndClassEx.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
		wndClassEx.lpszMenuName		= (isUseMenu ? MAKEINTRESOURCEW(IDC_WORLDWART_01) : NULL);
		wndClassEx.lpszClassName	= wndName.c_str();
		wndClassEx.hIconSm			= LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wndClassEx);



		//CreateWindow
		HWND hWnd = CreateWindowExW	(0L, wndName.c_str(), wndName.c_str(), wndStyle,
									wndPosX, wndPosY, wndSizeX, wndSizeY,
									GetDesktopWindow(), NULL, m_hInstance, NULL);
		if (!hWnd)
			return E_FAIL;



		//WindowResizing
		RECT windowResizing;
		SetRect(&windowResizing, 0, 0, wndSizeX, wndSizeY);
		AdjustWindowRectEx(&windowResizing, wndStyle, NULL, NULL);
		SetWindowPos(hWnd, NULL, 0, 0, windowResizing.right - windowResizing.left, windowResizing.bottom - windowResizing.top, SWP_NOZORDER | SWP_NOMOVE);



		//SetWindowInfo
		m_windowInfo.hWnd				= hWnd;

		m_windowInfo.name				= wndName;
		m_windowInfo.style				= wndStyle;

		m_windowInfo.wndSizeX			= wndSizeX;
		m_windowInfo.wndSizeY			= wndSizeY;
		m_windowInfo.clientAreaSizeX	= windowResizing.right	- windowResizing.left;
		m_windowInfo.clientAreaSizeY	= windowResizing.bottom	- windowResizing.top;
		m_windowInfo.posX				= wndPosX;
		m_windowInfo.posY				= wndPosY;

		m_windowInfo.isWindow			= true;
		m_windowInfo.isActive			= true;

		return S_OK;
	}

	HRESULT ProcessManager::CreateDevice()
	{
		LPDIRECT3D9	direct3D9;
		
		if (!(direct3D9 = Direct3DCreate9(D3D_SDK_VERSION)))
			return E_FAIL;

		D3DCAPS9 d3dCaps9;
		if (FAILED( direct3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps9) ))
			return E_FAIL;

		DWORD isDeviceVertexProcessing = d3dCaps9.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ?
			D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		
		D3DPRESENT_PARAMETERS d3dpp;
		d3dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;				//���� ���ۿ� ���׽� ���� ũ�� ���� 24bit ���ٽ� ���� 8 ��Ʈ
		d3dpp.BackBufferCount				= 1;						//����۴� �Ѱ�
		d3dpp.BackBufferFormat				= D3DFMT_A8R8G8B8;			//����� ���� ���� ����8��Ʈ,����8��Ʈ,�׸�8��Ʈ,���8��Ʈ
		d3dpp.BackBufferWidth				= m_windowInfo.wndSizeX;	//����� �ȼ� ���� ũ��
		d3dpp.BackBufferHeight				= m_windowInfo.wndSizeY;	//����� �ȼ� ���� ũ��
		d3dpp.EnableAutoDepthStencil		= true;						//�ڵ� ���̹��� ��� ���� ( �׳� true )
		d3dpp.Flags							= 0;						//�ϴ� 0
		d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;	//ȭ�� �ֻ��� ( �׳� D3DPRESENT_RATE_DEFAULT ����� �ֻ����� ���Ͻ� )
		d3dpp.hDeviceWindow					= m_windowInfo.hWnd;		//Device �� ��µ� ������ �ڵ�
		d3dpp.MultiSampleQuality			= 0;						//��Ƽ ���ø� �� 
		d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;		//��Ƽ ���ø� Ÿ�� ( D3DMULTISAMPLE_NONE ��Ƽ�˸��ƽ� ������ ) 
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_ONE;	//ȭ�� ���� ���� ( �׳� D3DPRESENT_INTERVAL_ONE ����� �ֻ����� ���Ͻ� )
		d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;	//ȭ�� ���� ü�� ��� ( D3DSWAPEFFECT_DISCARD )
		d3dpp.Windowed						= true;
		

		SetWindowLongPtr(m_windowInfo.hWnd, GWL_STYLE, m_windowInfo.style);
		SetWindowPos(m_windowInfo.hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_DRAWFRAME);
		ShowWindow(m_windowInfo.hWnd, m_cmdShow);
		


		HRESULT hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_windowInfo.hWnd, isDeviceVertexProcessing, &d3dpp, &m_d3dxDevice);
		SAFE_RELEASE(direct3D9);

		if (FAILED( hr ))
			return E_FAIL;
		return S_OK;
	}



	void ProcessManager::SetCallbackFunction(HRESULT(*initialize)(), HRESULT(*update)(), HRESULT(*render)(), HRESULT(*release)())
	{
		m_initialize = initialize;
		m_update = update;
		m_render = render;
		m_release = release;
	}

	int ProcessManager::ProcessRun()
	{
		if (m_initialize)
			m_initialize();

		MSG msg;
		PeekMessageW(&msg, NULL, NULL, NULL, PM_NOREMOVE);
		while (msg.message != WM_QUIT)
		{
			if (PeekMessageW(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if (m_update)
					m_update();

				if (m_d3dxDevice)
				{
					if (SUCCEEDED( m_d3dxDevice->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0x2b / 256.f, 0x65 / 256.f, 0xec / 256.f, 1), 1, 0) ) &&
						SUCCEEDED( m_d3dxDevice->BeginScene() ))
					{
						if (m_render)
							m_render();

						m_d3dxDevice->EndScene();
						m_d3dxDevice->Present(NULL, NULL, NULL, NULL);
					}
				}
				else if (m_render)
					m_render();
			}
		}

		if (m_release)
			m_release();

		return (int)msg.wParam;
	}

	LRESULT ProcessManager::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return g_processManager ? g_processManager->WndProc(hWnd, msg, wParam, lParam) : 0;
	}

	LRESULT ProcessManager::WndProc(const HWND & hWnd, const UINT & msg, const WPARAM & wParam, const LPARAM & lParam)
	{
		LRESULT returnValue;
		if (m_wndProc && (returnValue = m_wndProc(hWnd, msg, wParam, lParam)) != 0)
			return returnValue;
			
		switch (msg)
		{
		case WM_ACTIVATE:
			m_windowInfo.isActive = wParam;
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}
}
