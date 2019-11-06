#pragma once

enum class ButtonState
{
	Free,
	Clicked,
	Pressed,
	ClickOff,
};

class BaseButton
{
private:
	ButtonState m_state;

	D3DXVECTOR2 m_position;
	D3DXVECTOR2 m_collisionSize;

public:
	bool IsFree()		{ return m_state == ButtonState::Free; }
	bool IsClicked()	{ return m_state == ButtonState::Clicked; }
	bool IsPressed()	{ return m_state == ButtonState::Pressed || m_state == ButtonState::Clicked; }
	bool IsClickOff()	{ return m_state == ButtonState::ClickOff; }

public:
	void SetPosition(const D3DXVECTOR2& position) { m_position = position; }

public:
	void Update();

public:
	BaseButton(const D3DXVECTOR2& position, const D3DXVECTOR2& collisionSize);
	virtual ~BaseButton();
};
