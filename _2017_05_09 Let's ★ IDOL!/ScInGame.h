#pragma once
#include "Scene.h"

class ObjButton;
class CIdol;

class ScInGame :
	public CScene
{
private:
	enum BT_UI_MENU
	{
		HOME,
		EXIT,
		CREDIT,

		MENU_BT_END
	};
	enum BT_UI_KATEGORIE
	{
		CLOTHS,
		SHOES,
		EYES_BROW,
		MOUTH,
		HAIR,

		KATEGORIE_BT_END
	};

private:
	ObjButton*	m_uiMenuButton[MENU_BT_END];
	ObjButton*	m_uiKategorieButton[KATEGORIE_BT_END];

	ObjButton*	m_itemButton[5][6];

	CIdol*	m_character;

	int	m_nowSeletedType;


	bool	m_gameShutDown;

public:
	virtual	HRESULT	Init()		override;
	virtual	void	Update()	override;
	virtual	void	Render()	override;
	virtual	void	Release()	override;

public:
	ScInGame();
	~ScInGame();
};

