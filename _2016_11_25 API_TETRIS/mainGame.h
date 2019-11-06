#pragma once
#include "d2dManager.h"
#include "TETRIS MAP.h"

class mainGame
{
private:
	float tick;
	Tetris clTetris;
public:
	int iDownTimer;
	HRESULT init(void);
	void release(void);
	void update(float timeDelta);
	void render(HDC hdc);
	void vRenderBlock(char chBlockType, int iLocationX, int iLocationY);
	void vRenderBlock20(char chBlockType, int iLocationX, int iLocationY);

	void vStartTetris()
	{
		clTetris.vCreateTetris();
	}

	mainGame() {}
	~mainGame() {}
private:
	d2dManager* renderer;
};