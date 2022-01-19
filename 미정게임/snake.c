#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

// 게임 전체 틀의 크기
#define Row 24
#define Cols 22

// 게임 이동 키
#define Up 'w'
#define Down 's'
#define Left 'a'
#define Right 'd'

// 게임 부가기능 키
#define Die 'y'
#define Stop 'p'

// 커서의 좌표를 옮겨준다.
void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 커서를 없애준다.
void vCursor()
{
	CONSOLE_CURSOR_INFO curInfor;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfor);
	curInfor.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfor);
}

// 텍스트 배경 색 변경
void setcolor(int text, int back)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (text | back << 4));
}
// 게임 벽 안쪽을 리셋해준다.
void clrscr(int*cmap)
{
	int crow, ccols;
	gotoxy(1, 0);
	for (crow = 0; crow < Row - 3; crow++)
	{
		for (ccols = 0; ccols < Cols; ccols++)
		{
			if (*(cmap + ccols + (crow * Cols)) == 0 || *(cmap + ccols + (crow * Cols)) == 2)
			{
				printf("  ");
			}
		}
		gotoxy(2, crow + 1);
	}
}

// 게임 벽 생성
void Startmenu(int*pmap)
{
	int nrow, ncols;
	for (nrow = 0; nrow < Row; nrow++)
	{
		for (ncols = 0; ncols < Cols; ncols++)
		{
			if (*(pmap + ncols + (nrow * Cols)) == 1)
			{
				printf("▣");
			}
			else {
				printf("  ");
			}
		}
		printf("\n");
	}

}

// 점수 표시
void score(int score)
{
	gotoxy(9, 21);
	printf("점수 :");
	gotoxy(15, 21);
	printf("%d", score - 3);
}

// 뱀생성
void snake(int*smap)
{
	gotoxy(3, 1);
	int sx, sy, g = 0;
	for (sy = 0; sy < Row - 3; sy++)
	{
		for (sx = 0; sx < Cols; sx++)
		{
			if (*(smap + sx + (sy * Cols)) == 2)
			{
				setcolor(13, 0);
				printf("■");
			}
			else {
				if (*(smap + sx + (sy * Cols)) == 3)
				{
					setcolor(10, 0);
					printf("■");
				}
				else {
					if (*(smap + sx + (sy * Cols)) == 4)
					{
						setcolor(8, 0);
						printf("■");
					}
					else {
						if (*(smap + sx + (sy * Cols)) == 5)
						{
							setcolor(14, 0);
							printf("★");
						}
						else {
							if (*(smap + sx + (sy * Cols)) == 0) {
								printf("  ");
							}
						}
					}
				}
			}
		}
		gotoxy(2, sy + 1);
	}
	setcolor(15, 0);
}

// 코인 생성
void coin(int*pcoin, int*xcoin, int*ycoin)
{

	int cx = 17;
	srand(time(NULL));
	for (; ; cx++)
	{
		*xcoin = (rand() * cx) % 21;
		*ycoin = (rand() * cx) % 18;
		if (*(pcoin + *xcoin + (*ycoin * Cols)) == 0)
		{
			*(pcoin + *xcoin + (*ycoin * Cols)) = 5;
			break;
		}
	}
}

// 뱀 위쪽 이동
void upmove(int*upx, int*upy, int ulength)
{
	int upn;
	for (upn = 0; upn < ulength; upn++) {
		*(upx + upn) = *(upx + upn + 1);
		*(upy + upn) = *(upy + upn + 1);
	}
	*(upy + upn) -= 1;
}

// 뱀 아래쪽 이동
void downmove(int*downx, int*downy, int dlength)
{
	int downn;
	for (downn = 0; downn < dlength; downn++) {
		*(downx + downn) = *(downx + downn + 1);
		*(downy + downn) = *(downy + downn + 1);
	}
	*(downy + downn) += 1;
}

// 뱀 왼쪽 이동
void leftmove(int*leftx, int*lefty, int llength)
{
	int leftn;
	for (leftn = 0; leftn < llength; leftn++) {
		*(leftx + leftn) = *(leftx + leftn + 1);
		*(lefty + leftn) = *(lefty + leftn + 1);
	}
	*(leftx + leftn) -= 1;
}

// 뱀 오른쪽 이동
void rightmove(int*rightx, int*righty, int rlength)
{
	int rightn;
	for (rightn = 0; rightn < rlength; rightn++) {
		*(rightx + rightn) = *(rightx + rightn + 1);
		*(righty + rightn) = *(righty + rightn + 1);
	}
	*(rightx + rightn) += 1;
}

// 게임클리어
void clear(int*clear)
{
	int clx, cly;
	for (cly = 0; cly < Row - 3; cly++)
	{
		for (clx = 0; clx < Cols; clx++)
		{
			if (*(clear + clx + (cly * Cols)) == 2)
			{
				printf("ㅁ");
			}
		}
		gotoxy(2, cly + 1);
	}
}

// Ai
void ai(int*pai, int xcoin, int ycoin, int xsnake, int ysnake, int xtail, int ytail, char*abotton, int count)
{
	if (count == 0)
	{
		switch (*abotton)
		{
		case Up: {
			if (xcoin == xsnake)
			{
				if (ycoin > ysnake)
				{
					*abotton = Right;
					break;
				}
				else {
					if (ycoin < ysnake)
					{
						break;
					}
				}
			}
			else {
				if (xcoin < xsnake)
				{
					*abotton = Left;
					break;
				}
				else {
					if (xcoin > xsnake)
					{
						*abotton = Right;
						break;
					}
				}
			}
			break;
		}
		case Down: {
			if (xcoin == xsnake)
			{
				if (ycoin > ysnake)
				{
					break;
				}
				else {
					if (ycoin < ysnake)
					{
						*abotton = Right;
						break;
					}
				}
			}
			else {
				if (xcoin < xsnake)
				{
					*abotton = Left;
					break;
				}
				else {
					if (xcoin > xsnake)
					{
						*abotton = Right;
						break;
					}
				}
			}
			break;
		}
		case Left: {
			if (xcoin == xsnake)
			{
				if (ycoin > ysnake)
				{
					*abotton = Down;
					break;
				}
				else {
					if (ycoin < ysnake)
					{
						*abotton = Up;
						break;
					}
				}
			}
			else {
				if (xcoin < xsnake)
				{
					if (*(pai + xsnake + ((ysnake - 1) * Cols)) == 2 || *(pai + xsnake + ((ysnake - 1) * Cols)) == 1)
					{
						*abotton = Down;
					}
					else {
						if (*(pai + xsnake + ((ysnake + 1) * Cols)) == 2 || *(pai + xsnake + ((ysnake + 1) * Cols)) == 1)
							*abotton = Up;
					}
					break;
				}
				else {
					if (xcoin > xsnake)
					{
						*abotton = Down;
						break;
					}
				}
			}

			break;
		}
		case Right: {

			if (xcoin == xsnake)
			{
				if (ycoin > ysnake)
				{
					*abotton = Down;
					break;
				}
				else {
					if (ycoin < ysnake)
					{
						*abotton = Up;
						break;
					}
				}
			}
			else {
				if (xcoin < xsnake)
				{
					if (*(pai + xsnake + ((ysnake - 1) * Cols)) == 2 || *(pai + xsnake + ((ysnake - 1) * Cols)) == 1)
					{
						*abotton = Down;
					}
					else {
						if (*(pai + xsnake + ((ysnake + 1) * Cols)) == 2 || *(pai + xsnake + ((ysnake + 1) * Cols)) == 1)
							*abotton = Up;
					}
					break;
				}
				else {
					if (xcoin > xsnake)
					{
						break;
					}
				}
			}

			break;
		}
		default: {
			break;
		}
		}
	}
	else {
		switch (*abotton)
		{
		case Up: {
			if (xtail == xsnake)
			{
				if (ytail > ysnake)
				{
					*abotton = Right;
					break;
				}
				else {
					if (ytail < ysnake)
					{
						break;
					}
				}
			}
			else {
				if (xtail < xsnake)
				{
					*abotton = Left;
					break;
				}
				else {
					if (xtail > xsnake)
					{
						*abotton = Right;
						break;
					}
				}
			}
			break;
		}
		case Down: {
			if (xtail == xsnake)
			{
				if (ytail > ysnake)
				{
					break;
				}
				else {
					if (ytail < ysnake)
					{
						*abotton = Right;
						break;
					}
				}
			}
			else {
				if (xtail < xsnake)
				{
					*abotton = Left;
					break;
				}
				else {
					if (xtail > xsnake)
					{
						*abotton = Right;
						break;
					}
				}
			}
			break;
		}
		case Left: {
			if (xtail == xsnake)
			{
				if (ytail > ysnake)
				{
					*abotton = Down;
					break;
				}
				else {
					if (ytail < ysnake)
					{
						*abotton = Up;
						break;
					}
				}
			}
			else {
				if (xtail < xsnake)
				{
					if (*(pai + xsnake + ((ysnake - 1) * Cols)) == 2 || *(pai + xsnake + ((ysnake - 1) * Cols)) == 1)
					{
						*abotton = Down;
					}
					else {
						if (*(pai + xsnake + ((ysnake + 1) * Cols)) == 2 || *(pai + xsnake + ((ysnake + 1) * Cols)) == 1)
							*abotton = Up;
					}
					break;
				}
				else {
					if (xtail > xsnake)
					{
						*abotton = Down;
						break;
					}
				}
			}

			break;
		}
		case Right: {
			if (xtail == xsnake)
			{
				if (ycoin > ysnake)
				{
					*abotton = Down;
					break;
				}
				else {
					if (ycoin < ysnake)
					{
						*abotton = Up;
						break;
					}
				}
			}
			else {
				if (xtail < xsnake)
				{
					if (*(pai + xsnake + ((ysnake - 1) * Cols)) == 2 || *(pai + xsnake + ((ysnake - 1) * Cols)) == 1)
					{
						*abotton = Down;
					}
					else {
						if (*(pai + xsnake + ((ysnake + 1) * Cols)) == 2 || *(pai + xsnake + ((ysnake + 1) * Cols)) == 1)
							*abotton = Up;
					}
					break;
				}
				else {
					if (xtail > xsnake)
					{
						break;
					}
				}
			}

			break;
		}
		default: {
			break;
		}
		}
	}
	/*if ((table[bodyy[slength] + 1][bodyx[slength] + 1] == 4 || table[bodyy[slength] + 1][bodyx[slength] - 1] == 4 || table[bodyy[slength] - 1][bodyx[slength] - 1] == 4 || table[bodyy[slength] - 1][bodyx[slength] + 1] == 4 || table[bodyy[slength] - 1][bodyx[slength]] == 4 || table[bodyy[slength] + 1][bodyx[slength]] == 4 || table[bodyy[slength]][bodyx[slength] - 1] == 4 || table[bodyy[slength]][bodyx[slength] + 1] == 4) && temp > 0) {
		temp = 0;
		continue;
	}*/
}

int main()
{
	system("mode con: cols=44 Lines=25");
	system("title snake game");
	while (1) {
		vCursor();
		int slength = 3, speed = 150, coinx, coiny, temp = 0;
		char sbotton, mbotton, botton;
		botton = Right;
		int table[Row][Cols] = { { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,4,2,2,3,0,0,0,0,0,0,0,0,1 }, // table[12][9] ~ table[12][12]
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };
		int bodyx[360] = { 0 }, bodyy[360] = { 0 };
		for (int v = 0; v <= slength; v++) {
			bodyx[v] = 12 - slength + v;
			bodyy[v] = 12;
		}
		Startmenu(&table[0][0]);
		snake(&table[0][0]);
		gotoxy(17, 20);
		printf("GAME START");
		gotoxy(13, 22);
		printf("'S' botton press");
		gotoxy(11, 10);
		sbotton = 's';
		if (sbotton == 's' || sbotton == 'S') {
			coin(&table[0][0], &coinx, &coiny);
			while (1) {
				score(slength);
				Sleep(speed);
				gotoxy(10, 22);
				printf("%c", botton);
				gotoxy(13, 22);
				printf("%c", botton);
				if (_kbhit()) {
					mbotton = _getch();
					switch (mbotton) {
					case Up: {
						if (botton != Down) {
							if (table[bodyy[slength] - 1][bodyx[slength]] == 2 || table[bodyy[slength] - 1][bodyx[slength]] == 1) {
								mbotton = Die;
								break;
							}
							if (table[bodyy[slength] - 1][bodyx[slength]] == 5) {
								table[bodyy[slength] - 1][bodyx[slength]] = 3;
								table[bodyy[slength]][bodyx[slength]] = 2;
								coin(&table[0][0], &coinx, &coiny);
								slength += 1;
								bodyx[slength] = bodyx[slength - 1];
								bodyy[slength] = bodyy[slength - 1] - 1;
								botton = Up;
								break;
							}
							table[bodyy[0]][bodyx[0]] = 0;
							upmove(&bodyx[0], &bodyy[0], slength);
							for (int u = 0; u <= slength; u++)
							{
								if (u == 0)
								{
									table[bodyy[u]][bodyx[u]] = 4;
								}
								else
								{
									if (u == slength)
									{
										table[bodyy[u]][bodyx[u]] = 3;
									}
									else
									{
										table[bodyy[u]][bodyx[u]] = 2;
									}
								}
							}
							botton = Up;
							break;
						}
					}
					case Down: {
						if (botton != Up) {
							if (table[bodyy[slength] + 1][bodyx[slength]] == 2 || table[bodyy[slength] + 1][bodyx[slength]] == 1) {
								mbotton = Die;
								break;
							}
							if (table[bodyy[slength] + 1][bodyx[slength]] == 5) {
								table[bodyy[slength] + 1][bodyx[slength]] = 3;
								table[bodyy[slength]][bodyx[slength]] = 2;
								coin(&table[0][0], &coinx, &coiny);
								slength += 1;
								bodyx[slength] = bodyx[slength - 1];
								bodyy[slength] = bodyy[slength - 1] + 1;
								botton = Down;
								break;
							}
							table[bodyy[0]][bodyx[0]] = 0;
							downmove(&bodyx[0], &bodyy[0], slength);
							for (int u = 0; u <= slength; u++)
							{
								if (u == 0)
								{
									table[bodyy[u]][bodyx[u]] = 4;
								}
								else
								{
									if (u == slength)
									{
										table[bodyy[u]][bodyx[u]] = 3;
									}
									else
									{
										table[bodyy[u]][bodyx[u]] = 2;
									}
								}
							}
							botton = Down;
							break;
						}
					}
					case Left: {
						if (botton != Right) {
							if (table[bodyy[slength]][bodyx[slength] - 1] == 2 || table[bodyy[slength]][bodyx[slength] - 1] == 1) {
								mbotton = Die;
								break;
							}
							if (table[bodyy[slength]][bodyx[slength] - 1] == 5) {
								table[bodyy[slength]][bodyx[slength] - 1] = 3;
								table[bodyy[slength]][bodyx[slength]] = 2;
								coin(&table[0][0], &coinx, &coiny);
								slength += 1;
								bodyx[slength] = bodyx[slength - 1] - 1;
								bodyy[slength] = bodyy[slength - 1];
								botton = Left;
								break;
							}
							table[bodyy[0]][bodyx[0]] = 0;
							leftmove(&bodyx[0], &bodyy[0], slength);
							for (int u = 0; u <= slength; u++)
							{
								if (u == 0)
								{
									table[bodyy[u]][bodyx[u]] = 4;
								}
								else
								{
									if (u == slength)
									{
										table[bodyy[u]][bodyx[u]] = 3;
									}
									else
									{
										table[bodyy[u]][bodyx[u]] = 2;
									}
								}
							}
							botton = Left;
							break;
						}
					}
					case Right: {
						if (botton != Left) {
							if (table[bodyy[slength]][bodyx[slength] + 1] == 2 || table[bodyy[slength]][bodyx[slength] + 1] == 1) {
								mbotton = Die;
								break;
							}
							if (table[bodyy[slength]][bodyx[slength] + 1] == 5) {
								table[bodyy[slength]][bodyx[slength] + 1] = 3;
								table[bodyy[slength]][bodyx[slength]] = 2;
								coin(&table[0][0], &coinx, &coiny);
								slength += 1;
								bodyx[slength] = bodyx[slength - 1] + 1;
								bodyy[slength] = bodyy[slength - 1];
								botton = Right;
								break;
							}
							table[bodyy[0]][bodyx[0]] = 0;
							rightmove(&bodyx[0], &bodyy[0], slength);
							for (int u = 0; u <= slength; u++)
							{
								if (u == 0)
								{
									table[bodyy[u]][bodyx[u]] = 4;
								}
								else
								{
									if (u == slength)
									{
										table[bodyy[u]][bodyx[u]] = 3;
									}
									else
									{
										table[bodyy[u]][bodyx[u]] = 2;
									}
								}
							}
							botton = Right;
							break;
						}
					}
					default: {
						mbotton = botton;
						break;
					}
					}
				}
				else
				{
					switch (botton) {
					case Up: {
						if (table[bodyy[slength] - 1][bodyx[slength]] == 2 || table[bodyy[slength] - 1][bodyx[slength]] == 1) {
							mbotton = Die;
							break;
						}
						if (table[bodyy[slength] - 1][bodyx[slength]] == 5) {
							table[bodyy[slength] - 1][bodyx[slength]] = 3;
							table[bodyy[slength]][bodyx[slength]] = 2;
							coin(&table[0][0], &coinx, &coiny);
							slength += 1;
							bodyx[slength] = bodyx[slength - 1];
							bodyy[slength] = bodyy[slength - 1] - 1;
							botton = Up;
							break;
						}
						table[bodyy[0]][bodyx[0]] = 0;
						upmove(&bodyx[0], &bodyy[0], slength);
						for (int u = 0; u <= slength; u++)
						{
							if (u == 0)
							{
								table[bodyy[u]][bodyx[u]] = 4;
							}
							else
							{
								if (u == slength)
								{
									table[bodyy[u]][bodyx[u]] = 3;
								}
								else
								{
									table[bodyy[u]][bodyx[u]] = 2;
								}
							}
						}
						botton = Up;
						break;
					}
					case Down: {
						if (table[bodyy[slength] + 1][bodyx[slength]] == 2 || table[bodyy[slength] + 1][bodyx[slength]] == 1) {
							mbotton = Die;
							break;
						}
						if (table[bodyy[slength] + 1][bodyx[slength]] == 5) {
							table[bodyy[slength] + 1][bodyx[slength]] = 3;
							table[bodyy[slength]][bodyx[slength]] = 2;
							coin(&table[0][0], &coinx, &coiny);
							slength += 1;
							bodyx[slength] = bodyx[slength - 1];
							bodyy[slength] = bodyy[slength - 1] + 1;
							botton = Down;
							break;
						}
						table[bodyy[0]][bodyx[0]] = 0;
						downmove(&bodyx[0], &bodyy[0], slength);
						for (int u = 0; u <= slength; u++)
						{
							if (u == 0)
							{
								table[bodyy[u]][bodyx[u]] = 4;
							}
							else
							{
								if (u == slength)
								{
									table[bodyy[u]][bodyx[u]] = 3;
								}
								else
								{
									table[bodyy[u]][bodyx[u]] = 2;
								}
							}
						}
						botton = Down;
						break;
					}
					case Left: {
						if (table[bodyy[slength]][bodyx[slength] - 1] == 2 || table[bodyy[slength]][bodyx[slength] - 1] == 1) {
							mbotton = Die;
							break;
						}
						if (table[bodyy[slength]][bodyx[slength] - 1] == 5) {
							table[bodyy[slength]][bodyx[slength] - 1] = 3;
							table[bodyy[slength]][bodyx[slength]] = 2;
							coin(&table[0][0], &coinx, &coiny);
							slength += 1;
							bodyx[slength] = bodyx[slength - 1] - 1;
							bodyy[slength] = bodyy[slength - 1];
							botton = Left;
							break;
						}
						table[bodyy[0]][bodyx[0]] = 0;
						leftmove(&bodyx[0], &bodyy[0], slength);
						for (int u = 0; u <= slength; u++)
						{
							if (u == 0)
							{
								table[bodyy[u]][bodyx[u]] = 4;
							}
							else
							{
								if (u == slength)
								{
									table[bodyy[u]][bodyx[u]] = 3;
								}
								else
								{
									table[bodyy[u]][bodyx[u]] = 2;
								}
							}
						}
						botton = Left;
						break;
					}
					case Right: {
						if (table[bodyy[slength]][bodyx[slength] + 1] == 2 || table[bodyy[slength]][bodyx[slength] + 1] == 1) {
							mbotton = Die;
							break;
						}
						if (table[bodyy[slength]][bodyx[slength] + 1] == 5) {
							table[bodyy[slength]][bodyx[slength] + 1] = 3;
							table[bodyy[slength]][bodyx[slength]] = 2;
							coin(&table[0][0], &coinx, &coiny);
							slength += 1;
							bodyx[slength] = bodyx[slength - 1] + 1;
							bodyy[slength] = bodyy[slength - 1];
							botton = Right;
							break;
						}
						table[bodyy[0]][bodyx[0]] = 0;
						rightmove(&bodyx[0], &bodyy[0], slength);
						for (int u = 0; u <= slength; u++)
						{
							if (u == 0)
							{
								table[bodyy[u]][bodyx[u]] = 4;
							}
							else
							{
								if (u == slength)
								{
									table[bodyy[u]][bodyx[u]] = 3;
								}
								else
								{
									table[bodyy[u]][bodyx[u]] = 2;
								}
							}
						}
						botton = Right;
						break;
					}
					default: {
						break;
					}
					}
				}
				if (mbotton == Die)
				{
					break;
				}
				if (mbotton == Stop)
				{
					while (1)
					{
						mbotton = _getch();
						if (mbotton == 'S' || mbotton == 's')
						{
							break;
						}
					}
				}
				snake(&table[0][0]);
			}
		}
		while (1) {
			mbotton = '\0';
			clrscr(&table[0][0]);
			gotoxy(17, 10);
			printf("GAME OVER!");
			gotoxy(11, 11);
			printf("restart botton -> 'r'");
			gotoxy(9, 12);
			printf("quit Game botton - > 'q'");
			sbotton = _getch();
			if ((sbotton == 'r' || sbotton == 'R') || (sbotton == 'q' || sbotton == 'Q')) {
				break;
			}
			clrscr(&table[0][0]);
			gotoxy(13, 10);
			printf("<rewrite please>");
			Sleep(2000);
		}
		if (sbotton == 'r' || sbotton == 'R') {
			system("cls");
			continue;
		}
		else {
			if (sbotton == 'q' || sbotton == 'Q') {
				clrscr(&table[0][0]);
				gotoxy(17, 10);
				printf("Good Game!\n");
				Sleep(2000);
				exit(0);
			}
		}
	}
	return 0;
}
/*
if (bodyx[0] > bodyx[slength])
{
	botton = Right;
	for (int check = 1; check + bodyx[slength] < Cols; check++) {
		if (table[bodyy[slength]][bodyx[slength] + check] == 2)
		{
			botton = Left;
			for (int check2 = 1; check2 < bodyx[slength]; check2++)
			{
				if (table[bodyy[slength]][bodyx[slength] - check2] == 2)
				{
					if (check >= check2) {
						botton = Right;
						break;
					}
				}
			}
			break;
		}
	}
}
else {
	if (bodyx[0] <= bodyx[slength]) {
		botton = Left;
		for (int check = 1; check < bodyx[slength]; check++) {
			if (table[bodyy[slength]][bodyx[slength] - check] == 2)
			{
				botton = Right;
				for (int check2 = 1; check2 + bodyx[slength] < Cols; check2++)
				{
					if (table[bodyy[slength]][bodyx[slength] + check2] == 2)
					{
						if (check >= check2) {
							botton = Left;
							break;
						}
					}
				}
				break;
			}
		}
	}
}
temp++;
continue;
---------------------------------
if (bodyy[0] > bodyy[slength])
{
botton = Down;
for (int check = 1; check + bodyy[slength] < Row - 5; check++) {
if (table[bodyy[slength] + check][bodyx[slength]] == 2)
{
botton = Up;
for (int check2 = 1; check2 < bodyy[slength]; check2++) {
if (table[bodyy[slength] - check2][bodyx[slength]] == 2) {
if (check >= check2) {
botton = Down;
break;
}
}
}
break;
}
}
}
else {
if (bodyy[0] <= bodyy[slength]) {
botton = Up;
for (int check = 1; check < bodyy[slength]; check++) {
if (table[bodyy[slength] - check][bodyx[slength]] == 2)
{
botton = Down;
for (int check2 = 1; bodyy[slength] + check2 < Row - 5; check2++) {
if (table[bodyy[slength] + check2][bodyx[slength]] == 2) {
if (check >= check2) {
botton = Up;
break;
}
}
}
break;
}
}
}
}
temp++;
continue;
-----------------------
if (bodyy[0] > bodyy[slength])
{
botton = Down;
for (int check = 1; check + bodyy[slength] < Row - 5; check++) {
if (table[bodyy[slength] + check][bodyx[slength]] == 2)
{
botton = Up;
for (int check2 = 1; check2 < bodyy[slength]; check2++) {
if (table[bodyy[slength] - check2][bodyx[slength]] == 2) {
if (check >= check2) {
botton = Down;
break;
}
}
}
break;
}
}
}
else {
if (bodyy[0] <= bodyy[slength]) {
botton = Up;
for (int check = 1; check < bodyy[slength]; check++) {
if (table[bodyy[slength] - check][bodyx[slength]] == 2)
{
botton = Down;
for (int check2 = 1; bodyy[slength] + check2 < Row - 5; check2++) {
if (table[bodyy[slength] + check2][bodyx[slength]] == 2) {
if (check >= check2) {
botton = Up;
break;
}
}
}
break;
}
}
}
}
temp++;
continue;
------------------------
if (bodyx[0] > bodyx[slength])
{
botton = Right;
for (int check = 1; check + bodyx[slength] < Cols; check++) {
if (table[bodyy[slength]][bodyx[slength] + check] == 2)
{
botton = Left;
for (int check2 = 1; check2 < bodyx[slength]; check2++)
{
if (table[bodyy[slength]][bodyx[slength] - check2] == 2)
{
if (check >= check2) {
botton = Right;
break;
}
}
}
break;
}
}
}
else {
if (bodyx[0] <= bodyx[slength]) {
botton = Left;
for (int check = 1; check < bodyx[slength]; check++) {
if (table[bodyy[slength]][bodyx[slength] - check] == 2)
{
botton = Right;
for (int check2 = 1; check2 + bodyx[slength] < Cols; check2++)
{
if (table[bodyy[slength]][bodyx[slength] + check2] == 2)
{
if (check >= check2) {
botton = Left;
break;
}
}
}
}
}
}
}
temp++;
continue;
*/