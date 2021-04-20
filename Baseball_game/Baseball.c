#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <process.h>


#ifndef _COLOR_LIST_
#define _COLOR_LIST_

enum {
	black,
	blue,
	green,
	cyan,//mint
	red,
	purple,
	brown,
	lightgray,
	darkgray,
	lightblue,
	lightgreen,
	lightcyan,
	lightred,
	lightpurple,
	yellow,
	white
};

#endif


HANDLE g_hThread;
DWORD g_idThread;

DWORD __stdcall Animation_threadfunc(void);

void gotoXY(int x, int y);
void setColor(unsigned short color);

int main(void)
{
	system("mode con:cols=500 lines=500");
	system("color F");
	setColor(black);
	SetConsoleTitle(TEXT("Baseball_Game"));
	int answer = 0;
	do {
		system("cls");
		gotoXY(5, 5);
		printf("Welcome to Baseball Game!!");
		gotoXY(5, 7);
		printf("1. START");
		gotoXY(5, 8);
		printf("2. EXIT");
		gotoXY(5, 9);
		printf("Input : ");
		fflush(stdin);
		scanf_s("%d", &answer);
		getchar();
	} while (answer != 1 && answer != 2);

	if (answer == 2)
		return 1;
	system("cls");
	g_hThread = _beginthreadex(NULL,
		NULL,
		Animation_threadfunc,
		NULL,
		NULL,
		(unsigned int*)&g_idThread);

	int v, theta;
	do {
		gotoXY(50, 50);
		printf("v : ");
		scanf_s("%d", &v);
		getchar();
		printf("theta : ");
		scanf_s("%d", &theta);
		getchar();
	} while (v != -1 && theta != -1);

	WaitForSingleObject(g_hThread, INFINITE);
	puts("Press any key to exit");
	_getch();
	CloseHandle(g_hThread);

	return 0;
}

DWORD __stdcall Animation_threadfunc(void)
{

	return 0;
}

void gotoXY(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(unsigned short color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}