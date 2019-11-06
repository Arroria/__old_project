#include "stdafx.h"
#include "ProcessManager.h"
#include "Resource.h"



WndInfo::WndInfo()
	: hWnd(NULL)
	, name(L"")
	, wndStyle(NULL)

	, xSize(NULL)
	, ySize(NULL)
{
}



ProcessManager::ProcessManager(const HINSTANCE& hInstance)
	: m_hInstance(hInstance)
	, m_wndInfo(new WndInfo())

	, m_d3dxDevice(nullptr)

	, m_initialize(nullptr)
	, m_update(nullptr)
	, m_render(nullptr)
	, m_release(nullptr)
	, m_FPSLimite(NULL)
	, m_updateDeltaTime(NULL)
{
}
ProcessManager::~ProcessManager()
{
}



HRESULT ProcessManager::CreateWnd(const std::wstring & wndName, const DWORD & wndStyle, LRESULT(CALLBACK *wndProc)(HWND, UINT, WPARAM, LPARAM))
{
	//Register Window
	WNDCLASSEXW wcex;
	
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(107));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL; // MAKEINTRESOURCEW(IDC_SUBMARINE);
	wcex.lpszClassName = wndName.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	//Create Window
	m_wndInfo->hWnd = CreateWindowW(wndName.c_str(), wndName.c_str(), wndStyle,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInstance, nullptr);

	if (!m_wndInfo->hWnd)
		return E_FAIL;

	ShowWindow(m_wndInfo->hWnd, SW_RESTORE);
	//UpdateWindow(m_wndInfo->hWnd);

	m_wndInfo->name = wndName;
	m_wndInfo->wndStyle = wndStyle;
	return S_OK;
}

void ProcessManager::WndMove(const int & xPos, const int & yPos)
{
	SetWindowPos(m_wndInfo->hWnd, NULL, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void ProcessManager::WndResize(const unsigned int & xSize, const unsigned int & ySize)
{
	RECT temp;
	SetRect(&temp, 0, 0, xSize, ySize);
	AdjustWindowRect(&temp, m_wndInfo->wndStyle, NULL);
	SetWindowPos(m_wndInfo->hWnd, NULL, temp.left, temp.top, temp.right, temp.bottom, SWP_NOMOVE | SWP_NOZORDER);

	m_wndInfo->xSize = temp.right - temp.left;	//xSize;
	m_wndInfo->ySize = temp.bottom - temp.top;	//ySize;
}


HRESULT ProcessManager::CreateDevice(const bool & isWindow)
{
	LPDIRECT3D9	direct3D9;

	if (!(direct3D9 = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DCAPS9 d3dCaps9;
	if (FAILED(direct3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps9)))
		return E_FAIL;

	DWORD isDeviceVertexProcessing = d3dCaps9.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ?
		D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;															//뎁스 버퍼와 스테실 버퍼 크기 뎁스 24bit 스텐실 버퍼 8 비트
	d3dpp.BackBufferCount = 1;																	//백버퍼는 한개
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;														//백버퍼 색상 포맷 알파8비트,레드8비트,그린8비트,블루8비트
	d3dpp.BackBufferWidth = m_wndInfo->xSize;												//백버퍼 픽셀 가로 크기
	d3dpp.BackBufferHeight = m_wndInfo->ySize;												//백버퍼 픽셀 높이 크기
	d3dpp.EnableAutoDepthStencil = true;																	//자동 깊이버퍼 사용 여부 ( 그냥 true )
	d3dpp.Flags = 0;																	//일단 0
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;												//화면 주사율 ( 그냥 D3DPRESENT_RATE_DEFAULT 모니터 주사율과 동일시 )
	d3dpp.hDeviceWindow = m_wndInfo->hWnd;													//Device 가 출력될 윈도우 핸들
	d3dpp.MultiSampleQuality = 0;																	//멀티 샘플링 질 
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;													//멀티 샘플링 타입 ( D3DMULTISAMPLE_NONE 안티알리아싱 사용안함 ) 
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//(useVsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE);	//화면 전송 간격 ( 그냥 D3DPRESENT_INTERVAL_ONE 모니터 주사율과 동일시 )
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;												//화면 스왑 체인 방식 ( D3DSWAPEFFECT_DISCARD )
	d3dpp.Windowed = isWindow;

	///if (!isWindow)
	///{
	///	SetWindowLongPtr(m_wndInfo->hWnd, GWL_STYLE, m_wndInfo->wndStyle);
	///	SetWindowPos(m_wndInfo->hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_DRAWFRAME);
	///	ShowWindow(m_wndInfo->hWnd, SW_RESTORE);
	///}


	HRESULT hr = direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_wndInfo->hWnd, isDeviceVertexProcessing, &d3dpp, &m_d3dxDevice);
	SAFE_RELEASE(direct3D9);

	if (FAILED(hr))
		return E_FAIL;
	return S_OK;
}


WPARAM ProcessManager::Loop()
{
	//FPS Timer Manage variable
	__int64 updateTime;
	__int64 queryPerfomanceFrequency;
	__int64 prevUpdateTime;

	//Excution Initialize
	if (m_initialize)
	{
		if (m_initialize())
			return -1;
	}

	//FPS Timer correction
	QueryPerformanceFrequency((LARGE_INTEGER*)&queryPerfomanceFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&prevUpdateTime);

	MSG msg;
	PeekMessageW(&msg, NULL, NULL, NULL, PM_NOREMOVE);
	while (msg.message != WM_QUIT)
	{
		//Excution WndProc
		if (PeekMessageW(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			//Check FPS
			QueryPerformanceCounter((LARGE_INTEGER*)&updateTime);
			m_updateDeltaTime = (double)(updateTime - prevUpdateTime) / queryPerfomanceFrequency;
			if (!m_FPSLimite || m_updateDeltaTime >= 1.0 / m_FPSLimite)
			{
				prevUpdateTime = updateTime;

				//Excution Update
				if (m_update)
					m_update(m_updateDeltaTime);

				//Excution Render
				if (m_render)
				{
					if (m_d3dxDevice)
					{
						if (SUCCEEDED( m_d3dxDevice->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR(0x2b / 256.f, 0x65 / 256.f, 0xec / 256.f, 1), 1, 0) ) &&
							SUCCEEDED( m_d3dxDevice->BeginScene() ) )
						{
							bool ignoreDraw = m_render();

							m_d3dxDevice->EndScene();
							if (!ignoreDraw)
								m_d3dxDevice->Present(0, 0, 0, 0);
						}
					}
					else
						m_render();
				}
			}
		}
	}

	//Excution Release
	if (m_release)
	{
		if (m_release())
			return (msg.wParam == 0 ? -1 : msg.wParam);
	}

	return msg.wParam;
}
