#include "MainHeader.h"
#include "ItemSender.h"
#include "ItemManager.h"
#include "ASE.h"

namespace AF
{
	ItemSender::ItemSender(ItemManager* itemManager, const int& itemType, ASEModel* model, const D3DXVECTOR3& itemPos)
		: m_itemManager(itemManager)
		, m_아_영어몰라_배송중인아이템_타입임(itemType)
		, m_model(model)

		, m_isDelete(false)
		, m_temp(0)
	{
		m_position = itemPos;
		m_position.y = 500;
		m_rotation.y = 90;
	}
	ItemSender::~ItemSender()
	{
	}



	void AF::ItemSender::Initialize()
	{
	}

	void AF::ItemSender::Update()
	{
		m_position.y -= 3;
		if (m_position.y < 50)
		{
			m_itemManager->CreateItem(m_아_영어몰라_배송중인아이템_타입임, m_position);
			m_isDelete = true;
			return;
		}

		m_temp += D3DXToRadian(3);
		m_rotation.x = cosf(m_temp) * 20;
	}

	void AF::ItemSender::Render()
	{
		m_model->SetTransform(GetTransformMatrix());
		m_model->Update();
		m_model->Render();
	}

	void AF::ItemSender::Release()
	{
	}
}