#pragma once
#include "BaseObject.h"
namespace AF
{
	class ASEModel;

	class ItemManager;

	class ItemSender :
		public AF::BaseObject
	{
	private:
		ItemManager*	m_itemManager;

		ASEModel*		m_model;

		int				m_��_�������_������ξ�����_Ÿ����;

		bool			m_isDelete;
		float				m_temp;

	public:
		bool	IsDelete()	{ return m_isDelete; }

	public:
		virtual	void	Initialize	()	override;
		virtual	void	Update		()	override;
		virtual	void	Render		()	override;
		virtual	void	Release		()	override;

	public:
		ItemSender(ItemManager* itemManager, const int& itemType, ASEModel* model, const D3DXVECTOR3& itemPos);
		~ItemSender();
	};
}
