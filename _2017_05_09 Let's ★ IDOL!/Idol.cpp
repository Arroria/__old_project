#include "stdafx.h"
#include "Idol.h"


CIdol::CIdol()
	: m_characterBodyImage(nullptr)
	, m_itemImage{ nullptr }
	, m_nowWearingItem{ NULL }
{
}
CIdol::~CIdol()
{
}



void CIdol::Init()
{
	m_characterBodyImage = ResourceMgr->GetImage("CHAR_Body");

	char num[10];
	for (int i = 0; i < ITEM_KATEGORIE::ITEM_KATEGORIE_END; i++)
	{
		std::string	type;

		switch (i)
		{
		case ITEM_KATEGORIE::CLOTHS:	type = "CHAR_Cloth_";	break;
		case ITEM_KATEGORIE::SHOES:		type = "CHAR_Shoes_";	break;
		case ITEM_KATEGORIE::EYES_BROW:	type = "CHAR_Eye_";		break;
		case ITEM_KATEGORIE::MOUTH:		type = "CHAR_Mouth_";	break;
		case ITEM_KATEGORIE::HAIR:		type = "CHAR_Hair_";	break;
		}

		for (int j = 0; j < (j == ITEM_KATEGORIE::SHOES ? 4 : 6); j++)
		{
			m_itemImage[i][j] = ResourceMgr->GetImage(type + itoa(j + 1, num, 10));
		}
	}

	memset(&m_nowWearingItem, -1, sizeof(int) * 5);
}

void CIdol::Update()
{
}

void CIdol::Render()
{
	Renderer->SetWorld(VECTOR2(WIN_SIZEX * 0.5f - 128, WIN_SIZEY * 0.5f - 256 - 70));
	if (m_characterBodyImage) Renderer->Draw(m_characterBodyImage);

	for (int i = 0; i < ITEM_KATEGORIE::ITEM_KATEGORIE_END; i++)
	{
		if (m_nowWearingItem[i] != -1 && m_itemImage[i][m_nowWearingItem[i]])
			Renderer->Draw(m_itemImage[i][m_nowWearingItem[i]]);
	}
}

void CIdol::Release()
{
}
