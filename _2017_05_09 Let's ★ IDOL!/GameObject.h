#pragma once
class CGameObject
{
public:
	VECTOR2	m_pos;

public:
	void	SetPos(const VECTOR2& pos)	{ m_pos = pos; }
	VECTOR2	GetPos()					{ return m_pos; }

public:
	virtual void Init()		 = 0;	//초기화
	virtual void Update()	 = 0;	//업데이트
	virtual void Render()	 = 0;	//렌더
	virtual void Release()	 = 0;	//해제

public:
	CGameObject();
	virtual ~CGameObject();
};

