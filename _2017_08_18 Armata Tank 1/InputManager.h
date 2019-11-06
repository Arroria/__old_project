#pragma once
namespace AF
{
	class InputManager
	{
	private:
		enum KeyValue
		{
			Up,
			Down,
			None,
			Pressed,
		};

	private:
		bool	m_isWndActive;
		int		m_keyInput[256];

	private:
		void	UpdateKeyInput(const int& keyID);

	public:
		bool	IsKeyUp(const int& keyID) { return m_keyInput[keyID] == KeyValue::Up; }
		bool	IsKeyDown(const int& keyID) { return m_keyInput[keyID] == KeyValue::Down; }
		bool	IsKeyNone(const int& keyID) { return m_keyInput[keyID] == KeyValue::None || m_keyInput[keyID] == KeyValue::Up; }
		bool	IsKeyPressed(const int& keyID) { return m_keyInput[keyID] == KeyValue::Pressed || m_keyInput[keyID] == KeyValue::Down; }



	private:
		POINT	m_mousePos;
		POINT	m_mouseDelta;
		int		m_mouseWheel;

	public:
		const	POINT&	GetMousePos() { return m_mousePos; }
		const	POINT&	GetMouseDelta() { return m_mouseDelta; }
		const	int&	GetMouseWheel() { return m_mouseWheel; }



	public:
		void	Initialize();
		void	Update();
		void	Reset();
		bool	MsgProc(const HWND& hWnd, const UINT& msg, const WPARAM& wParam, const LPARAM& lParam, LRESULT& returnValue);

	public:
		InputManager();
		~InputManager();
	};

}
