#pragma once
#include "Scene.h"
class ScCredit :
	public CScene
{
public:
	virtual	HRESULT	Init()		override;
	virtual	void	Update()	override;
	virtual	void	Render()	override;
	virtual	void	Release()	override;

public:
	ScCredit();
	~ScCredit();
};

