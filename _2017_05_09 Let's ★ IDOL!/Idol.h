#pragma once
#include "GameObject.h"
class CIdol :
	public CGameObject
{
private:
	enum ITEM_KATEGORIE
	{
		CLOTHS,
		SHOES,
		EYES_BROW,
		MOUTH,
		HAIR,

		ITEM_KATEGORIE_END
	};

private:
	CBitmapImage*	m_characterBodyImage;

	int				m_nowWearingItem[5];
	CBitmapImage*	m_itemImage[5][6];

public:
	void	SetWearingItem(const int& type, const int& number) { m_nowWearingItem[type] = number; }

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

public:
	CIdol();
	~CIdol();
};

