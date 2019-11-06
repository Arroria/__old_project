#pragma once
#include "Singleton.h"
class BillBoard :
	public Singleton<BillBoard>
{
public:
	D3DXMATRIX Get();

public:
	BillBoard();
	~BillBoard();
};

#define BILLBOARD (SingletonInstance(BillBoard)->Get())

