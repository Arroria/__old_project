#pragma once
class CScene
{
protected:
	CBitmapImage*	m_sceneBackGroundImage;

public:
	virtual	HRESULT	Init()		PURE;
	virtual	void	Update()	PURE;
	virtual	void	Render()	PURE;
	virtual	void	Release()	PURE;

public:
	CScene();
	virtual ~CScene();
};

