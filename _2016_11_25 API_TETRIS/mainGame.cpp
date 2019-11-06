#include "stdafx.h"
#include "mainGame.h"

HRESULT mainGame::init(void)
{
	renderer = new d2dManager;
	renderer->init();

	//이미지 추가

	//UI
	renderer->addImage("Score", L"./image/UI/Score.png");

	//40블럭
	renderer->addImage("RedBlock", L"./image/Block/RedBlock.png");
	renderer->addImage("OrangeBlock", L"./image/Block/OrangeBlock.png");
	renderer->addImage("YellowBlock", L"./image/Block/YellowBlock.png");
	renderer->addImage("GreenBlock", L"./image/Block/GreenBlock.png");
	renderer->addImage("LightBlueBlock", L"./image/Block/LightBlueBlock.png");
	renderer->addImage("BlueBlock", L"./image/Block/BlueBlock.png");
	renderer->addImage("PurpleBlock", L"./image/Block/PurpleBlock.png");
	renderer->addImage("DarkBlock", L"./image/Block/DarkBlock.png");
	renderer->addImage("EE", L"./image/Block/EE.png");


	//20블럭
	renderer->addImage("RedBlock20", L"./image/Block20/RedBlock20.png");
	renderer->addImage("OrangeBlock20", L"./image/Block20/OrangeBlock20.png");
	renderer->addImage("YellowBlock20", L"./image/Block20/YellowBlock20.png");
	renderer->addImage("GreenBlock20", L"./image/Block20/GreenBlock20.png");
	renderer->addImage("LightBlueBlock20", L"./image/Block20/LightBlueBlock20.png");
	renderer->addImage("BlueBlock20", L"./image/Block20/BlueBlock20.png");
	renderer->addImage("PurpleBlock20", L"./image/Block20/PurpleBlock20.png");

	//넘버
	renderer->addImage("Number0", L"./image/Number/Number0.png");
	renderer->addImage("Number1", L"./image/Number/Number1.png");
	renderer->addImage("Number2", L"./image/Number/Number2.png");
	renderer->addImage("Number3", L"./image/Number/Number3.png");
	renderer->addImage("Number4", L"./image/Number/Number4.png");
	renderer->addImage("Number5", L"./image/Number/Number5.png");
	renderer->addImage("Number6", L"./image/Number/Number6.png");
	renderer->addImage("Number7", L"./image/Number/Number7.png");
	renderer->addImage("Number8", L"./image/Number/Number8.png");
	renderer->addImage("Number9", L"./image/Number/Number9.png");


	tick = 0;

	return S_OK;
}

void mainGame::release()
{
	renderer->release();
}

void mainGame::update(float timeDelta)
{
	//블럭이 없으면 생성
	if(clTetris.chBlockB == -1)
	{
		clTetris.vCreateBlock();
		iDownTimer = 0;
	}
	else
	{
		//키인식
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && tick < 0)
		{
			//좌측에 블럭이없다면 좌로 이동
			if (clTetris.chCheckCrashLeft())
			{
				clTetris.chBlockX--;
				tick = 0.15f;	//틱
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && tick < 0)
		{
			//우측에 블럭이없다면 우로 이동
			if (clTetris.chCheckCrashRight())
			{
				clTetris.chBlockX++;
				tick = 0.15f;	//틱
			}
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000 && tick < 0)
		{
			//블럭회전이 가능하다면 회전
			char chMoveX = clTetris.chCheckCrashRotation();
			if (chMoveX)
			{
				clTetris.chBlockR != 3 ? clTetris.chBlockR++ : clTetris.chBlockR = 0;

				//이동값을 적용
				clTetris.chBlockX += chMoveX - 2;

				tick = 0.15f;	//틱
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && tick < 0)
		{
			//하단에 블럭이없다면 아래로 이동
			if (clTetris.chCheckCrashDown())
			{
				clTetris.chBlockY++;
				tick = 0.15f;	//틱
			}
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && tick < 0)
		{
			//아래에 블럭이 있을때까지 블럭을 내림
			while (true)
			{
				if (clTetris.chCheckCrashDown())
					clTetris.chBlockY++;
				else
					break;
			}
			//블럭이 있다면 맵에 저장
			clTetris.vBlockInput();

			tick = 0.15f;	//틱
		}
		if (GetAsyncKeyState('S') & 0x8000 && tick < 0 && clTetris.chSaveC)
		{
			//블럭 저장함 ㅇㅇ
			clTetris.vBlockSave();
			iDownTimer = 0;
			tick = 0.15f;	//틱
		}
		if (GetAsyncKeyState('E') & 0x8000 && tick < 0)
		{
			clTetris.vEE();
		}
		tick -= timeDelta;

		//일정시간마다 블럭 내림
		if (iDownTimer >= 60)
		{
			//아래에 블럭이 없다면 블럭을 내림
			if (clTetris.chCheckCrashDown())
				clTetris.chBlockY++;
			//블럭이 있다면 맵에 저장
			else
			{
				clTetris.vBlockInput();
			}
			iDownTimer -= 60;
		}

		//다운 타이머 추가
		iDownTimer++;
	}
}

//화면 출력 함수
void mainGame::render(HDC hdc)
{
	//건들면 좆된다
	renderer->begin(hdc);

	//테트리스 UI출력
	//RECT Big = { 40,40,460,860 };	//큰 화면
	RECT Small = { 50,50,450,850 };	//작은 화면
	RECT wnd = { 0, 0, WINSIZEX, WINSIZEY };
	renderer->FillRectangle(wnd, RGB(255, 155, 155));//이거 지우지 마셈 더블버퍼링 대신임
	//renderer->FillRectangle(Big, RGB(255, 255, 255));
	renderer->FillRectangle(Small, RGB(200, 200, 200));

	//출력용 좌표 변수
	int iLocationX, iLocationY;

	//UI출력

	//테트리스 블럭 출력
	for (char chFr1 = 0; chFr1 < 4; chFr1++)
	{
		iLocationX = 10 + (clTetris.chBlockX + clTetris.chBlock[clTetris.chBlockB][clTetris.chBlockR][chFr1 * 2]) * 40;
		iLocationY = 50 + (clTetris.chBlockY + clTetris.chBlock[clTetris.chBlockB][clTetris.chBlockR][chFr1 * 2 + 1]) * 40;
		vRenderBlock(clTetris.chBlockB + 1 , iLocationX, iLocationY);
	}
	//NEXT 출력
	for (char chFr1 = 0; chFr1 < 4; chFr1++)
	{
		iLocationX = 510 + clTetris.chBlock[clTetris.chNextB][0][chFr1 * 2] * 20;
		iLocationY = 130 + clTetris.chBlock[clTetris.chNextB][0][chFr1 * 2 + 1] * 20;
		vRenderBlock20(clTetris.chNextB + 1 , iLocationX, iLocationY);
	}
	//SAVE 출력
	if(clTetris.chSaveB != -1)
	{
		for (char chFr1 = 0; chFr1 < 4; chFr1++)
		{
			iLocationX = 510 + clTetris.chBlock[clTetris.chSaveB][0][chFr1 * 2] * 20;
			iLocationY = 230 + clTetris.chBlock[clTetris.chSaveB][0][chFr1 * 2 + 1] * 20;
			vRenderBlock20(clTetris.chSaveB + 1 , iLocationX, iLocationY);
		}
	}

	//맵의 블럭들 출력 
	for (char chFr1 = 0; chFr1 < 20; chFr1++)
	{
		for (char chFr2 = 1; chFr2 < 11; chFr2++)
		{
			//블럭이 있다면 출력
			if (clTetris.chMap[chFr2][chFr1] > 0)
			{
				iLocationX = 10 + chFr2 * 40;
				iLocationY = 50 + chFr1 * 40;
				vRenderBlock(clTetris.chMap[chFr2][chFr1] , iLocationX, iLocationY);
			}
		}
	}

	renderer->end();
}

//블럭 출력
void mainGame::vRenderBlock(char chBlockType, int iLocationX, int iLocationY)
{
	switch(chBlockType)
	{
	case 1:
		renderer->render("RedBlock", iLocationX, iLocationY);
		break;
	case 2:
		renderer->render("OrangeBlock", iLocationX, iLocationY);
		break;
	case 3:
		renderer->render("YellowBlock", iLocationX, iLocationY);
		break;
	case 4:
		renderer->render("GreenBlock", iLocationX, iLocationY);
		break;
	case 5:
		renderer->render("LightBlueBlock", iLocationX, iLocationY);
		break;
	case 6:
		renderer->render("BlueBlock", iLocationX, iLocationY);
		break;
	case 7:
		renderer->render("PurpleBlock", iLocationX, iLocationY);
		break;
	case 8:
		renderer->render("DarkBlock", iLocationX, iLocationY);
		break;
	case 22:
		renderer->render("EE", iLocationX, iLocationY);
		break;
	}
}

void mainGame::vRenderBlock20(char chBlockType, int iLocationX, int iLocationY)
{
	switch(chBlockType)
	{
	case 1:
		renderer->render("RedBlock20", iLocationX, iLocationY);
		break;
	case 2:
		renderer->render("OrangeBlock20", iLocationX, iLocationY);
		break;
	case 3:
		renderer->render("YellowBlock20", iLocationX, iLocationY);
		break;
	case 4:
		renderer->render("GreenBlock20", iLocationX, iLocationY);
		break;
	case 5:
		renderer->render("LightBlueBlock20", iLocationX, iLocationY);
		break;
	case 6:
		renderer->render("BlueBlock20", iLocationX, iLocationY);
		break;
	case 7:
		renderer->render("PurpleBlock20", iLocationX, iLocationY);
		break;
	}
}