#pragma once
#include "Singleton.h"
class BillBoarder :
	public Singleton<BillBoarder>
{
private:
	D3DXMATRIX m_billBoardM;

public:
	void Update();
	D3DXMATRIX GetMatrix() { return m_billBoardM; }

public:
	BillBoarder();
	~BillBoarder();
};

