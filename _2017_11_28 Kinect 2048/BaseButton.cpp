#include "stdafx.h"
#include "BaseButton.h"

#define g_inputManager GetSingleton(InputManager)

BaseButton::BaseButton(const D3DXVECTOR2& position, const D3DXVECTOR2& collisionSize)
	: m_state(ButtonState::Free)
	
	, m_position(position)
	, m_collisionSize(collisionSize)
{
}
BaseButton::~BaseButton()
{
}


void BaseButton::Update()
{
	auto MouseCollisionCheck = [](const POINT_ex& mousePos, const RECT_ex& buttonCollisionRect)->bool
	{
		return
			buttonCollisionRect.left	< mousePos.x &&
			buttonCollisionRect.right	> mousePos.x &&
			buttonCollisionRect.top		< mousePos.y &&
			buttonCollisionRect.bottom	> mousePos.y;
	};
	RECT_ex buttonCollisionRect(POINT_ex(m_collisionSize.x, m_collisionSize.y));
	buttonCollisionRect += POINT_ex(m_position.x, m_position.y);
	switch (m_state)
	{
	case ButtonState::Free:
		if (g_inputManager->IsKeyDown(VK_LBUTTON) && MouseCollisionCheck(g_inputManager->GetMousePos(), buttonCollisionRect))
			m_state = ButtonState::Clicked;
		break;
	case ButtonState::Clicked:
		if (g_inputManager->IsKeyPressed(VK_LBUTTON))
			m_state = ButtonState::Pressed;
		else if (MouseCollisionCheck(g_inputManager->GetMousePos(), buttonCollisionRect))
			m_state = ButtonState::ClickOff;
		else
			m_state = ButtonState::Free;
		break;
	case ButtonState::Pressed:
		if (!g_inputManager->IsKeyPressed(VK_LBUTTON))
		{
			if (MouseCollisionCheck(g_inputManager->GetMousePos(), buttonCollisionRect))
				m_state = ButtonState::ClickOff;
			else
				m_state = ButtonState::Free;
		}
		break;
	case ButtonState::ClickOff:
		if (g_inputManager->IsKeyDown(VK_LBUTTON) && MouseCollisionCheck(g_inputManager->GetMousePos(), buttonCollisionRect))
			m_state = ButtonState::Clicked;
		else
			m_state = ButtonState::Free;
		break;
	default:
		break;
	}
}
