#pragma once
#include "GameObject.h"
class ObjButton :
	public CGameObject
{
private:
	enum BT_INFO
	{
		Default,
		Click,
		Clicked,
	};

private:
	CBitmapImage*	m_btImage[3];
	int				m_btInfo;
	std::string		m_defaultKey;
	int				m_clickEventTickCount;

	bool			m_clickImageOff;
	bool			m_clickedImageOff;

	bool			m_buttonIsDown;

public:
	void	SetDefaultImageKey(const std::string& key) { m_defaultKey = key; }
	bool	GetButtonIsClicked() { return m_buttonIsDown; }

	void	SetClickImageOff() { m_clickImageOff = true; }
	void	SetClickedImageOff() { m_clickedImageOff = true; }

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

public:
	ObjButton();
	~ObjButton();
};

