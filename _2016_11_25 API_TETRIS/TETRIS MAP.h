#include "stdafx.h"
#include <time.h>

class Tetris {
public:
	char chBlock[7][4][8];// ㅗ

	char chMap[12][21];//맵

	//블럭
	char chBlockB;//블럭    -1 = 블럭없음
	char chBlockX;//x좌표
	char chBlockY;//y좌표
	char chBlockR;//회전값

	//NEXT 블럭
	char chNextB;//블럭

	//SAVE 블럭
	char chSaveB;//블럭    -1 = 블럭없음
	char chSaveC;//가능여부

	//점수 저장
	int iScore;

	//블럭다운 저장
	char chBlockDownCount;

	//테트리스 정의
	void vCreateTetris()
	{
		//시드 랜덤화
		srand((unsigned int)time(NULL));

		//블럭 생성
		char temp[7][4][8] = { {//블럭 정의
		{ 2,0,2,1,2,2,2,3 },{ 0,1,1,1,2,1,3,1 },{ 2,0,2,1,2,2,2,3 },{ 0,1,1,1,2,1,3,1 } },{// I
		{ 1,1,2,1,1,2,2,2 },{ 1,1,2,1,1,2,2,2 },{ 1,1,2,1,1,2,2,2 },{ 1,1,2,1,1,2,2,2 } },{// ㅁ
		{ 1,0,1,1,1,2,2,2 },{ 1,1,2,1,3,1,1,2 },{ 1,1,2,1,2,2,2,3 },{ 2,1,0,2,1,2,2,2 } },{// L
		{ 2,0,2,1,1,2,2,2 },{ 1,1,1,2,2,2,3,2 },{ 1,1,2,1,1,2,1,3 },{ 0,1,1,1,2,1,2,2 } },{// L(R)
		{ 0,0,0,1,1,1,1,2 },{ 1,0,2,0,0,1,1,1 },{ 0,0,0,1,1,1,1,2 },{ 1,0,2,0,0,1,1,1 } },{// S
		{ 1,0,0,1,1,1,0,2 },{ 0,0,1,0,1,1,2,1 },{ 1,0,0,1,1,1,0,2 },{ 0,0,1,0,1,1,2,1 } },{// S(R)
		{ 1,0,0,1,1,1,2,1 },{ 1,0,1,1,2,1,1,2 },{ 0,1,1,1,2,1,1,2 },{ 1,0,0,1,1,1,1,2 } } };
		//이거유한이갓임
		memcpy(chBlock, temp, 7*4*8);

		//맵 설정
		vCreateMap();
		
		//점수 초기화
		iScore = 0;

		//NEXT블럭 미리생성
		chNextB = rand() % 7;
		
		//SAVE기능 활성화
		chSaveB = -1;
		chSaveC = 1;

		//점수 초기화
		iScore = 0;

		//블럭다운 카운터 초기화
		chBlockDownCount = 0;
	}

	//맵 기본설정
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
	//사용 블럭 생성
	void vCreateBlock()
	{
		//NEXT블럭 불러옴
		chBlockB = chNextB;
		chBlockR = 0;
		chBlockX = 4;
		chBlockY = 0;

		//NEXT블럭 생성
		chNextB = rand() % 7;
	}

	//블럭 좌 충돌 확인    1 = 이동가능
	char chCheckCrashLeft()
	{
		//4개의 블럭을 각각검사
		for (char chFr1 = 0; chFr1 < 4; chFr1++)
		{
			//충돌 확인시 충돌 반환
			if (chMap[chBlock[chBlockB][chBlockR][chFr1 * 2] + chBlockX - 1][chBlock[chBlockB][chBlockR][chFr1 * 2 + 1] + chBlockY] != 0)
				return 0;
		}
		//충돌이 없으면 비충돌 반환
		return 1;
	}
	//블럭 우 충돌 확인
	char chCheckCrashRight()
	{
		//4개의 블럭을 각각검사
		for (char chFr1 = 0; chFr1 < 4; chFr1++)
		{
			//충돌 확인시 충돌 반환
			if (chMap[chBlock[chBlockB][chBlockR][chFr1 * 2] + chBlockX + 1][chBlock[chBlockB][chBlockR][chFr1 * 2 + 1] + chBlockY] != 0)
				return 0;
		}
		//충돌이 없으면 비충돌 반환
		return 1;
	}
	//블럭 하단 충돌 확인
	char chCheckCrashDown()
	{
		//4개의 블럭을 각각검사
		for (char chFr1 = 0; chFr1 < 4; chFr1++)
		{
			//충돌 확인시 충돌 반환
			if (chMap[chBlock[chBlockB][chBlockR][chFr1 * 2] + chBlockX][chBlock[chBlockB][chBlockR][chFr1 * 2 + 1] + chBlockY + 1] != 0)
				return 0;
		}
		//충돌이 없으면 비충돌 반환
		return 1;
	}
	//블럭 회전 충돌 확인
	char chCheckCrashRotation()
	{
		//가변 회전값 추가
		char chRotationValue;
		chBlockR != 3 ? chRotationValue = 1 : chRotationValue = -3;

		//충돌 저장용 변수 생성
		char chCheckBreak = -1;
		//벽 충돌시 X좌표 이동값
		char chMoveX = 0;

		//벽과의 충돌이 없을때까지 반복
		while(chCheckBreak == -1 || chCheckBreak == 2 && chMoveX != -1 && chMoveX != 2)
		{
			chCheckBreak = 0;
			if(chBlockX != -1)
			{
				//4개의 블럭을 각각검사
				for (char chFr1 = 0; chFr1 < 4; chFr1++)
				{
					//좌우의 벽과의 충돌이 있을경우 저장
					if (chMap[chBlock[chBlockB][chBlockR + chRotationValue][chFr1 * 2] + chBlockX + chMoveX][chBlock[chBlockB][chBlockR + chRotationValue][chFr1 * 2 + 1] + chBlockY] == -1)
					{
						chCheckBreak = 2;
						break;
					}
					//충돌이 있을경우 저장
					else if (chMap[chBlock[chBlockB][chBlockR + chRotationValue][chFr1 * 2] + chBlockX + chMoveX][chBlock[chBlockB][chBlockR + chRotationValue][chFr1 * 2 + 1] + chBlockY] > 0)
						chCheckBreak = 1;
				}
			}
			else
				chCheckBreak = 2;
			//벽과의 충돌이 있었을경우 블럭 위치에 따라 이동값 변경
			if(chCheckBreak == 2)
			{
				if(chBlockX == -1)
					chMoveX+=2;
				else
					chBlockX <= 4 ? chMoveX++ : chMoveX--;
			}
		}

		//충돌이 있었다면 충돌반환
		if(chCheckBreak == 1)
			return 0;
		//충돌이 없었다면 이동값+2 (비충돌) 반환
		else
			return chMoveX + 2;
	}

	//현재 블럭 맵에 저장
	void vBlockInput()
	{
		//4개의 블럭에 모두 실행
		for (char chFr1 = 0; chFr1 < 4; chFr1++)
			//맵에 블럭 저장
			chMap[chBlock[chBlockB][chBlockR][chFr1 * 2] + chBlockX][chBlock[chBlockB][chBlockR][chFr1 * 2 + 1] + chBlockY] = chBlockB + 1;
		//블럭상태 없음으로 저장
		chBlockB = -1;

		//맵 갱신
		vCheckLine();
		//게임오버 체크
		vGameoverCheck();

		//블럭 저장 가능하게함
		chSaveC = 1;
		
		//블럭다운 카운터 갱신및 체크
		chBlockDownCount++;
		vBlockDownPenalty();
	}
	//현재 블럭 임시 저장
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

	//줄 확인 및 제거
	void vCheckLine()
	{
		//검사 대상 줄을 한칸씩 올림
		for (char chCheckLineY = 19; chCheckLineY >= 0; chCheckLineY--)
		{
			//현재 줄이 완성됬는지 확인
			char chCheckLine = 1;
			for (char chCheckLineX = 1; chCheckLineX < 11; chCheckLineX++)
			{
				if (chMap[chCheckLineX][chCheckLineY] == 0)
				{
					chCheckLine = 0;
					break;
				}
			}
			//줄이 완성됬을경우
			if (chCheckLine)
			{
				//줄 초기화
				for (char chRemoveLineX = 1; chRemoveLineX < 11; chRemoveLineX++)
					chMap[chRemoveLineX][chCheckLineY] = 0;
				//윗줄들을 모두 내림
				for (char chChangeLineY = chCheckLineY - 1; chChangeLineY >= 0; chChangeLineY--)
				{
					for (char chChangeLineX = 1; chChangeLineX < 11; chChangeLineX++)
					{
						//블럭이 있다면 내림
						if (chMap[chChangeLineX][chChangeLineY] != 0)
						{
							chMap[chChangeLineX][chChangeLineY + 1] = chMap[chChangeLineX][chChangeLineY];
							chMap[chChangeLineX][chChangeLineY] = 0;
						}
					}
				}
				//줄 재검사
				chCheckLineY++;
			}
		}
	}
	//블럭다운 패널티
	void vBlockDownPenalty(){
		//패널티를 받을 상황이 되면
		if(chBlockDownCount >= 10)
		{
			//맵 한칸씩 올림
			for(char chMapY = 1; chMapY < 20; chMapY++)
			{
				for(char chMapX = 1; chMapX < 11; chMapX++)
					chMap[chMapX][chMapY - 1] = chMap[chMapX][chMapY];
			}
			//맨 아랫줄 블럭생성
			for(char chMapX = 1; chMapX < 11; chMapX++)
				chMap[chMapX][19] = 8;
			//블럭 하나만 없앰
			chMap[rand() % 10 + 1][19] = 0;

			//패널티 제거
			chBlockDownCount -= 10;
		}

		//게임오버 체크
		vGameoverCheck();
	}


	//게임오버 체크
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

	//점수갱신
	void vScoreChange(char chScoreType){
		switch (chScoreType)
		{
		//블럭다운
		case 0:
			iScore += 20;
			break;
		//줄제거
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

	//생성자 소멸자
	Tetris(){}
	~Tetris(){}

	//EE
	void vEE(){
		chMap[2][15] = 22, chMap[3][15] = 22, chMap[4][15] = 22, chMap[7][15] = 22, chMap[8][15] = 22, chMap[9][15] = 22, chMap[2][16] = 22, chMap[7][16] = 22, chMap[2][17] = 22, chMap[3][17] = 22, chMap[4][17] = 22, chMap[7][17] = 22, chMap[8][17] = 22, chMap[9][17] = 22, chMap[2][18] = 22, chMap[7][18] = 22, chMap[2][19] = 22, chMap[3][19] = 22, chMap[4][19] = 22, chMap[7][19] = 22, chMap[8][19] = 22, chMap[9][19] = 22;
	}
};