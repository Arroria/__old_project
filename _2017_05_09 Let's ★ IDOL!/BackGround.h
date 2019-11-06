#pragma once
#include "GameObject.h"
class CBackGround :
	public CGameObject
{
public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;

public:
	CBackGround();
	virtual ~CBackGround();
};

