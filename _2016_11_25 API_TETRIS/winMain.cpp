//��������� ����ϰڽ��ϴ�

#include "stdafx.h"
#include "mainGame.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

//���������� ����Ѵ�
HINSTANCE _hInstance;
HWND _hWnd;

POINT _ptMouse = { 0, 0 };

//���ΰ��� ��������
mainGame _mg;

//�Լ� ������Ÿ�� ����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	//�õ� ����ȭ
	srand((unsigned int)time(NULL));

	HDC hdc;
	MSG message;		//�ü������ �߻��ϴ� �޼��� ������ �����ϱ� ���� ����ü
	WNDCLASS wndClass;	//�������� ������ �����ϱ� ���� ����ü

	_hInstance = hInstance;

	wndClass.cbClsExtra = 0;										//Ŭ���� ���� �޸�
	wndClass.cbWndExtra = 0;										//������ ���� �޸�
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//������ ��׶��� ����
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//������ Ŀ��
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//������ ������
	wndClass.hInstance = hInstance;									//������ �ν��Ͻ�
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//������ ���ν���
	wndClass.lpszClassName = WINNAME;								//Ŭ���� �̸�
	wndClass.lpszMenuName = NULL;									//�޴��̸�
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//������ ������

	//������ Ŭ���� ���
	RegisterClass(&wndClass);

	//â ũ�� ����ȭ
	RECT WINDOWSIZE = { 0,0,800,940 };
	AdjustWindowRect(&WINDOWSIZE, WINSTYLE, false);

	//������ ���帶ũ �Ǽ�
	_hWnd = CreateWindow(
		WINNAME,				//������ Ŭ���� �̸�
		WINNAME,				//������ Ÿ��Ʋ �ٿ� ��Ÿ�� ���ڿ�
		WS_CAPTION | WS_SYSMENU,	//������ ��Ÿ��
		WINSTARTX,				//������ â ���� X��ǥ
		WINSTARTY,				//������ â ���� Y��ǥ
		WINDOWSIZE.right,				//������ â ����ũ��
		WINDOWSIZE.bottom,				//������ â ����ũ��
		NULL,					//�θ� ������ ����
		(HMENU)NULL,			//�޴��ڵ�
		hInstance,				//�ν��Ͻ� ������ ����
		NULL);

	//������ â�� ����ش�
	ShowWindow(_hWnd, cmdShow);
	hdc = GetDC(_hWnd);

	if (FAILED(_mg.init()))
	{
		//���ΰ����� �ʱ�ȭ �����ϸ� �ٷ� �����Ŵ
		return 0;
	}
	bool _isHardware;//���� �ý��� ��������
	__int64 _periodFrequency;

	float _timeScale = 0.001f;
	__int64 _curTime;
	__int64 _lastTime;

	const float dt = 1.0f/ 60.0f;//���� ������(1�� / 60FPS / timeScale)
	float _mTime = 0.0f;//���� �ִ� �ð�
	float _gameTime = 0.0f;
	int _FPS = 0;
	float _FPSTime = 0.0f;
	bool _update = false;

	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))//��ȯ���� bool���� �Լ��� ����, ���� Ÿ�̸� ������ �Ǵ��� Ȯ���ϴ� �Լ��� ����.
	{
		_isHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);//������ �ȴٸ�, ���� �ð��� �ҷ��´�.

		_timeScale = 1.0f / _periodFrequency;//�Ʒ��ʿ� �и������� ������ �ʸ� ������ ����ٴ°����� ����, _periodFrequency�� �ʴ� ȭ���� �󸶳� ���ŵɼ� �ִ����� �ҷ��µ� �ϴ�.
	}
	else
	{
		_isHardware = false;

		//�и� ������ ������ �ʸ� ������ ����
		_lastTime = timeGetTime();//�����ð��� �ҷ��´�.
		_timeScale = 0.001f;//ȭ�鰻�ű����� �ð�
	}

	////����.
	_mg.vStartTetris();

	while (true)
	{
		//�޼��� ����
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
				break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			_isHardware ? QueryPerformanceCounter((LARGE_INTEGER*)&_curTime) : _curTime = timeGetTime();//���׿�����
			_mTime += (_curTime - _lastTime) * _timeScale;
			_lastTime = _curTime;

			while (_mTime >= dt)
			{
				_mg.update(dt);

					
				//Ÿ�̸� ����
				_mTime -= dt;
				_gameTime += dt;
				_update = true;
				_FPSTime += dt;
				_FPS++;
			}
			if (_update)
			{
				//����� ����

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

	//�޽��� �� �� �� ����
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