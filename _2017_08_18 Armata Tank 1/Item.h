#pragma once
#include "BaseObject.h"
namespace AF
{
	class ASEModel;

	class Item :
		public AF::BaseObject
	{
	private:
		int			m_itemType;
		ASEModel*	m_model;

		int			m_deleteTime;
		int			m_prevTime;

	public:
		void	SetDelete() { m_deleteTime = 0; }
		bool	IsDelete() { return m_deleteTime <= 0; }


	public:
		int		GetItemType() { return m_itemType; }

	public:
		virtual	void	Initialize()	override;
		virtual	void	Update()		override;
		virtual	void	Render()		override;
		virtual	void	Release()		override;

	public:
		Item(const int& itemType, ASEModel* model);
		~Item();
	};
}

