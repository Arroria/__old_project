#include "stdafx.h"
#include "mainGame.h"

HRESULT mainGame::init(void)
{
	renderer = new d2dManager;
	renderer->init();

	//�̹��� �߰�

	//UI
	renderer->addImage("Score", L"./image/UI/Score.png");

	//40��
	renderer->addImage("RedBlock", L"./image/Block/RedBlock.png");
	renderer->addImage("OrangeBlock", L"./image/Block/OrangeBlock.png");
	renderer->addImage("YellowBlock", L"./image/Block/YellowBlock.png");
	renderer->addImage("GreenBlock", L"./image/Block/GreenBlock.png");
	renderer->addImage("LightBlueBlock", L"./image/Block/LightBlueBlock.png");
	renderer->addImage("BlueBlock", L"./image/Block/BlueBlock.png");
	renderer->addImage("PurpleBlock", L"./image/Block/PurpleBlock.png");
	renderer->addImage("DarkBlock", L"./image/Block/DarkBlock.png");
	renderer->addImage("EE", L"./image/Block/EE.png");


	//20��
	renderer->addImage("RedBlock20", L"./image/Block20/RedBlock20.png");
	renderer->addImage("OrangeBlock20", L"./image/Block20/OrangeBlock20.png");
	renderer->addImage("YellowBlock20", L"./image/Block20/YellowBlock20.png");
	renderer->addImage("GreenBlock20", L"./image/Block20/GreenBlock20.png");
	renderer->addImage("LightBlueBlock20", L"./image/Block20/LightBlueBlock20.png");
	renderer->addImage("BlueBlock20", L"./image/Block20/BlueBlock20.png");
	renderer->addImage("PurpleBlock20", L"./image/Block20/PurpleBlock20.png");

	//�ѹ�
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
	//���� ������ ����
	if(clTetris.chBlockB == -1)
	{
		clTetris.vCreateBlock();
		iDownTimer = 0;
	}
	else
	{
		//Ű�ν�
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && tick < 0)
		{
			//������ ���̾��ٸ� �·� �̵�
			if (clTetris.chCheckCrashLeft())
			{
				clTetris.chBlockX--;
				tick = 0.15f;	//ƽ
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && tick < 0)
		{
			//������ ���̾��ٸ� ��� �̵�
			if (clTetris.chCheckCrashRight())
			{
				clTetris.chBlockX++;
				tick = 0.15f;	//ƽ
			}
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000 && tick < 0)
		{
			//��ȸ���� �����ϴٸ� ȸ��
			char chMoveX = clTetris.chCheckCrashRotation();
			if (chMoveX)
			{
				clTetris.chBlockR != 3 ? clTetris.chBlockR++ : clTetris.chBlockR = 0;

				//�̵����� ����
				clTetris.chBlockX += chMoveX - 2;

				tick = 0.15f;	//ƽ
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000 && tick < 0)
		{
			//�ϴܿ� ���̾��ٸ� �Ʒ��� �̵�
			if (clTetris.chCheckCrashDown())
			{
				clTetris.chBlockY++;
				tick = 0.15f;	//ƽ
			}
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && tick < 0)
		{
			//�Ʒ��� ���� ���������� ���� ����
			while (true)
			{
				if (clTetris.chCheckCrashDown())
					clTetris.chBlockY++;
				else
					break;
			}
			//���� �ִٸ� �ʿ� ����
			clTetris.vBlockInput();

			tick = 0.15f;	//ƽ
		}
		if (GetAsyncKeyState('S') & 0x8000 && tick < 0 && clTetris.chSaveC)
		{
			//�� ������ ����
			clTetris.vBlockSave();
			iDownTimer = 0;
			tick = 0.15f;	//ƽ
		}
		if (GetAsyncKeyState('E') & 0x8000 && tick < 0)
		{
			clTetris.vEE();
		}
		tick -= timeDelta;

		//�����ð����� �� ����
		if (iDownTimer >= 60)
		{
			//�Ʒ��� ���� ���ٸ� ���� ����
			if (clTetris.chCheckCrashDown())
				clTetris.chBlockY++;
			//���� �ִٸ� �ʿ� ����
			else
			{
				clTetris.vBlockInput();
			}
			iDownTimer -= 60;
		}

		//�ٿ� Ÿ�̸� �߰�
		iDownTimer++;
	}
}

//ȭ�� ��� �Լ�
void mainGame::render(HDC hdc)
{
	//�ǵ�� ���ȴ�
	renderer->begin(hdc);

	//��Ʈ���� UI���
	//RECT Big = { 40,40,460,860 };	//ū ȭ��
	RECT Small = { 50,50,450,850 };	//���� ȭ��
	RECT wnd = { 0, 0, WINSIZEX, WINSIZEY };
	renderer->FillRectangle(wnd, RGB(255, 155, 155));//�̰� ������ ���� ������۸� �����
	//renderer->FillRectangle(Big, RGB(255, 255, 255));
	renderer->FillRectangle(Small, RGB(200, 200, 200));

	//��¿� ��ǥ ����
	int iLocationX, iLocationY;

	//UI���

	//��Ʈ���� �� ���
	for (char chFr1 = 0; chFr1 < 4; chFr1++)
	{
		iLocationX = 10 + (clTetris.chBlockX + clTetris.chBlock[clTetris.chBlockB][clTetris.chBlockR][chFr1 * 2]) * 40;
		iLocationY = 50 + (clTetris.chBlockY + clTetris.chBlock[clTetris.chBlockB][clTetris.chBlockR][chFr1 * 2 + 1]) * 40;
		vRenderBlock(clTetris.chBlockB + 1 , iLocationX, iLocationY);
	}
	//NEXT ���
	for (char chFr1 = 0; chFr1 < 4; chFr1++)
	{
		iLocationX = 510 + clTetris.chBlock[clTetris.chNextB][0][chFr1 * 2] * 20;
		iLocationY = 130 + clTetris.chBlock[clTetris.chNextB][0][chFr1 * 2 + 1] * 20;
		vRenderBlock20(clTetris.chNextB + 1 , iLocationX, iLocationY);
	}
	//SAVE ���
	if(clTetris.chSaveB != -1)
	{
		for (char chFr1 = 0; chFr1 < 4; chFr1++)
		{
			iLocationX = 510 + clTetris.chBlock[clTetris.chSaveB][0][chFr1 * 2] * 20;
			iLocationY = 230 + clTetris.chBlock[clTetris.chSaveB][0][chFr1 * 2 + 1] * 20;
			vRenderBlock20(clTetris.chSaveB + 1 , iLocationX, iLocationY);
		}
	}

	//���� ���� ��� 
	for (char chFr1 = 0; chFr1 < 20; chFr1++)
	{
		for (char chFr2 = 1; chFr2 < 11; chFr2++)
		{
			//���� �ִٸ� ���
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

//�� ���
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