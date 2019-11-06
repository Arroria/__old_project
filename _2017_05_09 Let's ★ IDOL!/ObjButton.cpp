#include "stdafx.h"
#include "ObjButton.h"


ObjButton::ObjButton()
	: m_btImage{ nullptr }
	, m_btInfo(BT_INFO::Default)
	, m_buttonIsDown(false)
	, m_clickEventTickCount(0)
	, m_clickImageOff(false)
	, m_clickedImageOff(false)
{
}
ObjButton::~ObjButton()
{
}


void ObjButton::Init()
{
	m_btImage[BT_INFO::Default] =	ResourceMgr->GetImage(m_defaultKey);
	if (!m_clickImageOff)
		m_btImage[BT_INFO::Click] =		ResourceMgr->GetImage(m_defaultKey + "_Click");
	if (!m_clickedImageOff)
		m_btImage[BT_INFO::Clicked] =	ResourceMgr->GetImage(m_defaultKey + "_Clicked");
}

void ObjButton::Update()
{
	POINT mouse = KeyMgr->GetMousePos();

	m_buttonIsDown = false;


	if ((m_btImage[m_btInfo] ||
		(m_btInfo == BT_INFO::Click && m_clickImageOff) ||
		(m_btInfo == BT_INFO::Clicked && m_clickedImageOff)))
	{
		if (KeyMgr->MouseUp() &&
			m_pos.x < mouse.x && mouse.x < m_pos.x + m_btImage[m_btInfo]->width &&
			m_pos.y < mouse.y && mouse.y < m_pos.y + m_btImage[m_btInfo]->height)
		{
			m_clickEventTickCount = GetTickCount();
			m_buttonIsDown = true;
		}



		if (KeyMgr->MousePressed() && !m_clickImageOff &&
			m_pos.x < mouse.x && mouse.x < m_pos.x + m_btImage[m_btInfo]->width &&
			m_pos.y < mouse.y && mouse.y < m_pos.y + m_btImage[m_btInfo]->height)
			m_btInfo = BT_INFO::Click;
		else if (m_clickEventTickCount + 250 > GetTickCount() && !m_clickedImageOff)
			m_btInfo = BT_INFO::Clicked;
		else
			m_btInfo = BT_INFO::Default;
	}
}

void ObjButton::Render()
{
	if (m_btImage[m_btInfo])
	{
		ResourceMgr->SetWorld(m_pos);
		ResourceMgr->Draw(m_btImage[m_btInfo]);
	}
}

void ObjButton::Release()
{
}
