#include "stdafx.h"
#include <time.h>

class Tetris {
public:
	char chBlock[7][4][8];// ��

	char chMap[12][21];//��

	//��
	char chBlockB;//��    -1 = ������
	char chBlockX;//x��ǥ
	char chBlockY;//y��ǥ
	char chBlockR;//ȸ����

	//NEXT ��
	char chNextB;//��

	//SAVE ��
	char chSaveB;//��    -1 = ������
	char chSaveC;//���ɿ���

	//���� ����
	int iScore;

	//���ٿ� ����
	char chBlockDownCount;

	//��Ʈ���� ����
	void vCreateTetris()
	{
		//�õ� ����ȭ
		srand((unsigned int)time(NULL));

		//�� ����
		char temp[7][4][8] = { {//�� ����
		{ 2,0,2,1,2,2,2,3 },{ 0,1,1,1,2,1,3,1 },{ 2,0,2,1,2,2,2,3 },{ 0,1,1,1,2,1,3,1 } },{// I
		{ 1,1,2,1,1,2,2,2 },{ 1,1,2,1,1,2,2,2 },{ 1,1,2,1,1,2,2,2 },{ 1,1,2,1,1,2,2,2 } },{// ��
		{ 1,0,1,1,1,2,2,2 },{ 1,1,2,1,3,1,1,2 },{ 1,1,2,1,2,2,2,3 },{ 2,1,0,2,1,2,2,2 } },{// L
		{ 2,0,2,1,1,2,2,2 },{ 1,1,1,2,2,2,3,2 },{ 1,1,2,1,1,2,1,3 },{ 0,1,1,1,2,1,2,2 } },{// L(R)
		{ 0,0,0,1,1,1,1,2 },{ 1,0,2,0,0,1,1,1 },{ 0,0,0,1,1,1,1,2 },{ 1,0,2,0,0,1,1,1 } },{// S
		{ 1,0,0,1,1,1,0,2 },{ 0,0,1,0,1,1,2,1 },{ 1,0,0,1,1,1,0,2 },{ 0,0,1,0,1,1,2,1 } },{// S(R)
		{ 1,0,0,1,1,1,2,1 },{ 1,0,1,1,2,1,1,2 },{ 0,1,1,1,2,1,1,2 },{ 1,0,0,1,1,1,1,2 } } };
		//�̰������̰���
		memcpy(chBlock, temp, 7*4*8);

		//�� ����
		vCreateMap();
		
		//���� �ʱ�ȭ
		iScore = 0;

		//NEXT�� �̸�����
		chNextB = rand() % 7;
		
		//SAVE��� Ȱ��ȭ
		chSaveB = -1;
		chSaveC = 1;

		//���� �ʱ�ȭ
		iScore = 0;

		//���ٿ� ī���� �ʱ�ȭ
		chBlockDownCount = 0;
	}

	//�� �⺻����
	void vCreateMap()
	{
		for(char chFr1 = 0; chFr1 < 20; chFr1++)
		{
			for(char chFr2 = 0; chFr2 < 12; chFr2++)
			{
				if(chFr2 == 0 || chFr2 == 11)
					chMap[chFr2][chFr1] = -1;
				else
					chMap[chFr2][chFr1] = 0;
			}
		}
		for(char chFr1 = 0; chFr1 < 12; chFr1++)
			chMap[chFr1][20] = -2;
	}
	//��� �� ����
	void vCreateBlock()
	{
		//NEXT�� �ҷ���
		chBlockB = chNextB;
		chBlockR = 0;
		chBlockX = 4;
		chBlockY = 0;

		//NEXT�� ����
		chNextB = rand() % 7;
	}

	//�� �� �浹 Ȯ��    1 = �̵�����
	char chCheckCrashLeft()
	{
		//4���� ���� �����˻�
		for (char chFr1 = 0; chFr1 < 4; chFr1++)
		{
			//�浹 Ȯ�ν� �浹 ��ȯ
			if (chMap[chBlock[chBlockB][chBlockR][chFr1 * 2] + chBlockX - 1][chBlock[chBlockB][chBlockR][chFr1 * 2 + 1] + chBlockY] != 0)
				return 0;
		}
		//�浹�� ������ ���浹 ��ȯ
		return 1;
	}
	//�� �� �浹 Ȯ��
	char chCheckCrashRight()
	{
		//4���� ���� �����˻�
		for (char chFr1 = 0; chFr1 < 4; chFr1++)
		{
			//�浹 Ȯ�ν� �浹 ��ȯ
			if (chMap[chBlock[chBlockB][chBlockR][chFr1 * 2] + chBlockX + 1][chBlock[chBlockB][chBlockR][chFr1 * 2 + 1] + chBlockY] != 0)
				return 0;
		}
		//�浹�� ������ ���浹 ��ȯ
		return 1;
	}
	//�� �ϴ� �浹 Ȯ��
	char chCheckCrashDown()
	{
		//4���� ���� �����˻�
		for (char chFr1 = 0; chFr1 < 4; chFr1++)
		{
			//�浹 Ȯ�ν� �浹 ��ȯ
			if (chMap[chBlock[chBlockB][chBlockR][chFr1 * 2] + chBlockX][chBlock[chBlockB][chBlockR][chFr1 * 2 + 1] + chBlockY + 1] != 0)
				return 0;
		}
		//�浹�� ������ ���浹 ��ȯ
		return 1;
	}
	//�� ȸ�� �浹 Ȯ��
	char chCheckCrashRotation()
	{
		//���� ȸ���� �߰�
		char chRotationValue;
		chBlockR != 3 ? chRotationValue = 1 : chRotationValue = -3;

		//�浹 ����� ���� ����
		char chCheckBreak = -1;
		//�� �浹�� X��ǥ �̵���
		char chMoveX = 0;

		//������ �浹�� ���������� �ݺ�
		while(chCheckBreak == -1 || chCheckBreak == 2 && chMoveX != -1 && chMoveX != 2)
		{
			chCheckBreak = 0;
			if(chBlockX != -1)
			{
				//4���� ���� �����˻�
				for (char chFr1 = 0; chFr1 < 4; chFr1++)
				{
					//�¿��� ������ �浹�� ������� ����
					if (chMap[chBlock[chBlockB][chBlockR + chRotationValue][chFr1 * 2] + chBlockX + chMoveX][chBlock[chBlockB][chBlockR + chRotationValue][chFr1 * 2 + 1] + chBlockY] == -1)
					{
						chCheckBreak = 2;
						break;
					}
					//�浹�� ������� ����
					else if (chMap[chBlock[chBlockB][chBlockR + chRotationValue][chFr1 * 2] + chBlockX + chMoveX][chBlock[chBlockB][chBlockR + chRotationValue][chFr1 * 2 + 1] + chBlockY] > 0)
						chCheckBreak = 1;
				}
			}
			else
				chCheckBreak = 2;
			//������ �浹�� �־������ �� ��ġ�� ���� �̵��� ����
			if(chCheckBreak == 2)
			{
				if(chBlockX == -1)
					chMoveX+=2;
				else
					chBlockX <= 4 ? chMoveX++ : chMoveX--;
			}
		}

		//�浹�� �־��ٸ� �浹��ȯ
		if(chCheckBreak == 1)
			return 0;
		//�浹�� �����ٸ� �̵���+2 (���浹) ��ȯ
		else
			return chMoveX + 2;
	}

	//���� �� �ʿ� ����
	void vBlockInput()
	{
		//4���� ���� ��� ����
		for (char chFr1 = 0; chFr1 < 4; chFr1++)
			//�ʿ� �� ����
			chMap[chBlock[chBlockB][chBlockR][chFr1 * 2] + chBlockX][chBlock[chBlockB][chBlockR][chFr1 * 2 + 1] + chBlockY] = chBlockB + 1;
		//������ �������� ����
		chBlockB = -1;

		//�� ����
		vCheckLine();
		//���ӿ��� üũ
		vGameoverCheck();

		//�� ���� �����ϰ���
		chSaveC = 1;
		
		//���ٿ� ī���� ���Ź� üũ
		chBlockDownCount++;
		vBlockDownPenalty();
	}
	//���� �� �ӽ� ����
	void vBlockSave()
	{
		if(chSaveB == -1)
		{
			chSaveB = chBlockB;
			chBlockB = -1;
		}
		else
		{
			char chSaveBlock = chSaveB;
			chSaveB = chBlockB;
			chBlockB = chSaveBlock;

			chBlockX = 4;
			chBlockY = 0;
		}
		chSaveC = 0;
	}

	//�� Ȯ�� �� ����
	void vCheckLine()
	{
		//�˻� ��� ���� ��ĭ�� �ø�
		for (char chCheckLineY = 19; chCheckLineY >= 0; chCheckLineY--)
		{
			//���� ���� �ϼ������ Ȯ��
			char chCheckLine = 1;
			for (char chCheckLineX = 1; chCheckLineX < 11; chCheckLineX++)
			{
				if (chMap[chCheckLineX][chCheckLineY] == 0)
				{
					chCheckLine = 0;
					break;
				}
			}
			//���� �ϼ��������
			if (chCheckLine)
			{
				//�� �ʱ�ȭ
				for (char chRemoveLineX = 1; chRemoveLineX < 11; chRemoveLineX++)
					chMap[chRemoveLineX][chCheckLineY] = 0;
				//���ٵ��� ��� ����
				for (char chChangeLineY = chCheckLineY - 1; chChangeLineY >= 0; chChangeLineY--)
				{
					for (char chChangeLineX = 1; chChangeLineX < 11; chChangeLineX++)
					{
						//���� �ִٸ� ����
						if (chMap[chChangeLineX][chChangeLineY] != 0)
						{
							chMap[chChangeLineX][chChangeLineY + 1] = chMap[chChangeLineX][chChangeLineY];
							chMap[chChangeLineX][chChangeLineY] = 0;
						}
					}
				}
				//�� ��˻�
				chCheckLineY++;
			}
		}
	}
	//���ٿ� �г�Ƽ
	void vBlockDownPenalty(){
		//�г�Ƽ�� ���� ��Ȳ�� �Ǹ�
		if(chBlockDownCount >= 10)
		{
			//�� ��ĭ�� �ø�
			for(char chMapY = 1; chMapY < 20; chMapY++)
			{
				for(char chMapX = 1; chMapX < 11; chMapX++)
					chMap[chMapX][chMapY - 1] = chMap[chMapX][chMapY];
			}
			//�� �Ʒ��� ������
			for(char chMapX = 1; chMapX < 11; chMapX++)
				chMap[chMapX][19] = 8;
			//�� �ϳ��� ����
			chMap[rand() % 10 + 1][19] = 0;

			//�г�Ƽ ����
			chBlockDownCount -= 10;
		}

		//���ӿ��� üũ
		vGameoverCheck();
	}


	//���ӿ��� üũ
	void vGameoverCheck()
	{
		char chGameoverCheck = 0;
		for(char chFr1 = 1; chFr1 < 11; chFr1++)
		{
			if(chMap[chFr1][5])
			{
				chGameoverCheck = 1;
				break;
			}
		}
		if(chGameoverCheck)
			PostQuitMessage(0);
	}

	//��������
	void vScoreChange(char chScoreType){
		switch (chScoreType)
		{
		//���ٿ�
		case 0:
			iScore += 20;
			break;
		//������
		case 1:
		case 2:
		case 3:
		case 4:
			{
			short shPlusScore = 100 * (chScoreType + 4) * chScoreType;
			iScore += shPlusScore;
			break;
			}
		}
	}

	//������ �Ҹ���
	Tetris(){}
	~Tetris(){}

	//EE
	void vEE(){
		chMap[2][15] = 22, chMap[3][15] = 22, chMap[4][15] = 22, chMap[7][15] = 22, chMap[8][15] = 22, chMap[9][15] = 22, chMap[2][16] = 22, chMap[7][16] = 22, chMap[2][17] = 22, chMap[3][17] = 22, chMap[4][17] = 22, chMap[7][17] = 22, chMap[8][17] = 22, chMap[9][17] = 22, chMap[2][18] = 22, chMap[7][18] = 22, chMap[2][19] = 22, chMap[3][19] = 22, chMap[4][19] = 22, chMap[7][19] = 22, chMap[8][19] = 22, chMap[9][19] = 22;
	}
};