//헤더파일을 사용하겠습니다

#include "stdafx.h"
#include "mainGame.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

//전역변수를 사용한다
HINSTANCE _hInstance;
HWND _hWnd;

POINT _ptMouse = { 0, 0 };

//메인게임 전역선언
mainGame _mg;

//함수 프로토타입 선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	//시드 랜덤화
	srand((unsigned int)time(NULL));

	HDC hdc;
	MSG message;		//운영체제에서 발생하는 메세지 정보를 저장하기 위한 구조체
	WNDCLASS wndClass;	//윈도우의 정보를 저장하기 위한 구조체

	_hInstance = hInstance;

	wndClass.cbClsExtra = 0;										//클래스 여분 메모리
	wndClass.cbWndExtra = 0;										//윈도우 여분 메모리
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//윈도우 백그라운드 색상
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//윈도우 커서
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//윈도우 아이콘
	wndClass.hInstance = hInstance;									//윈도우 인스턴스
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//윈도우 프로시져
	wndClass.lpszClassName = WINNAME;								//클래스 이름
	wndClass.lpszMenuName = NULL;									//메뉴이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//윈도우 스따일

	//윈도우 클래스 등록
	RegisterClass(&wndClass);

	//창 크기 정밀화
	RECT WINDOWSIZE = { 0,0,800,940 };
	AdjustWindowRect(&WINDOWSIZE, WINSTYLE, false);

	//윈도우 랜드마크 건설
	_hWnd = CreateWindow(
		WINNAME,				//윈도우 클래스 이름
		WINNAME,				//윈도우 타이틀 바에 나타낼 문자열
		WS_CAPTION | WS_SYSMENU,	//윈도우 스타일
		WINSTARTX,				//윈도우 창 시작 X좌표
		WINSTARTY,				//윈도우 창 시작 Y좌표
		WINDOWSIZE.right,				//윈도우 창 가로크기
		WINDOWSIZE.bottom,				//윈도우 창 세로크기
		NULL,					//부모 윈도우 유무
		(HMENU)NULL,			//메뉴핸들
		hInstance,				//인스턴스 윈도우 지정
		NULL);

	//윈도우 창을 띄워준다
	ShowWindow(_hWnd, cmdShow);
	hdc = GetDC(_hWnd);

	if (FAILED(_mg.init()))
	{
		//메인게임이 초기화 실패하면 바로 종료시킴
		return 0;
	}
	bool _isHardware;//고성능 시스템 지원여부
	__int64 _periodFrequency;

	float _timeScale = 0.001f;
	__int64 _curTime;
	__int64 _lastTime;

	const float dt = 1.0f/ 60.0f;//기준 프레임(1초 / 60FPS / timeScale)
	float _mTime = 0.0f;//남아 있는 시간
	float _gameTime = 0.0f;
	int _FPS = 0;
	float _FPSTime = 0.0f;
	bool _update = false;

	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))//반환값이 bool값인 함수로 봐서, 고성능 타이머 지원이 되는지 확인하는 함수로 보임.
	{
		_isHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);//지원이 된다면, 최종 시간을 불러온다.

		_timeScale = 1.0f / _periodFrequency;//아래쪽에 밀리세컨드 단위로 초를 강제로 맞춘다는것으로 보아, _periodFrequency는 초당 화면이 얼마나 갱신될수 있는지를 불러온듯 하다.
	}
	else
	{
		_isHardware = false;

		//밀리 세컨드 단위로 초를 강제로 맞춤
		_lastTime = timeGetTime();//최종시간을 불러온다.
		_timeScale = 0.001f;//화면갱신까지의 시간
	}

	////게임.
	_mg.vStartTetris();

	while (true)
	{
		//메세지 여부
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
				break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			_isHardware ? QueryPerformanceCounter((LARGE_INTEGER*)&_curTime) : _curTime = timeGetTime();//삼항연산자
			_mTime += (_curTime - _lastTime) * _timeScale;
			_lastTime = _curTime;

			while (_mTime >= dt)
			{
				_mg.update(dt);

					
				//타이머 관련
				_mTime -= dt;
				_gameTime += dt;
				_update = true;
				_FPSTime += dt;
				_FPS++;
			}
			if (_update)
			{
				//블럭출력 번경

				_update = false;
				_mg.render(hdc);
			}
			if (_FPSTime > 1.0f)
			{
				//DEBUG_LOG(_FPS);
				_FPS = 0;
				_FPSTime -= 1.0f;
			}
		}
	}

	//메시지 다 돈 후 해제
	_mg.release();
	ReleaseDC(_hWnd, hdc);


	UnregisterClass(WINNAME, hInstance);
	_CrtDumpMemoryLeaks();

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_MOUSEMOVE:
		_ptMouse.x = static_cast<float>(LOWORD(lParam));
		_ptMouse.y = static_cast<float>(HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}