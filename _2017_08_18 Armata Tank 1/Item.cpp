#include "MainHeader.h"
#include "Item.h"

#include "ASE.h"

namespace AF
{
	Item::Item(const int& itemType, ASEModel* model)
		: m_itemType(itemType)
		, m_model(model)
		, m_deleteTime(15000)
		, m_prevTime(GetTickCount())
	{
		m_scale *= 0.3;
	}
	Item::~Item()
	{
	}



	void Item::Initialize()
	{
	}

	void Item::Update()
	{
		int	nowTime = GetTickCount();
		m_deleteTime -= nowTime - m_prevTime;

		m_prevTime = nowTime;

		m_rotation.y += 5;
	}

	void Item::Render()
	{
		m_model->SetTransform(GetTransformMatrix());
		m_model->Update();
		m_model->Render();
	}

	void Item::Release()
	{
	}

}
