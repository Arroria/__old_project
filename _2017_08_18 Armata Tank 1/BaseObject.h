#pragma once
namespace AF
{
	class BaseObject
	{
	protected:
		D3DXVECTOR3	m_position;
		D3DXVECTOR3	m_rotation;
		D3DXVECTOR3	m_scale;

		D3DXVECTOR3 m_direction;


	public:
		D3DXVECTOR3	GetPosition	()	{ return m_position; }
		D3DXVECTOR3	GetRotation	()	{ return m_rotation; }
		D3DXVECTOR3	GetScale	()	{ return m_scale; }
		void		SetPosition	(const D3DXVECTOR3& position)	{ m_position	= position;}
		void		SetRotation	(const D3DXVECTOR3& rotation)	{ m_rotation	= rotation;}
		void		SetScale	(const D3DXVECTOR3& scale)		{ m_scale		= scale;}

		D3DXVECTOR3	GetDirection();
		void		SetDirection(const D3DXVECTOR3& direction)	{ m_direction = direction; }

	public:
		D3DXMATRIX	GetTransformMatrix();


	public:
		virtual	void	Initialize()	PURE;
		virtual	void	Update()		PURE;
		virtual	void	Render()		PURE;
		virtual	void	Release()		PURE;


	public:
		BaseObject();
		virtual ~BaseObject();
	};
}
