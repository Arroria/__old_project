#pragma once
class CGameObject
{
public:
	VECTOR2	m_pos;

public:
	void	SetPos(const VECTOR2& pos)	{ m_pos = pos; }
	VECTOR2	GetPos()					{ return m_pos; }

public:
	virtual void Init()		 = 0;	//�ʱ�ȭ
	virtual void Update()	 = 0;	//������Ʈ
	virtual void Render()	 = 0;	//����
	virtual void Release()	 = 0;	//����

public:
	CGameObject();
	virtual ~CGameObject();
};

