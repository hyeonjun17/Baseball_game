#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <process.h>

HANDLE hThread;
DWORD idThread;

DWORD __stdcall Animation_threadfunc(void);

void gotoXY(int x, int y);

int main(void)
{
	int answer = 0;
	do {
		system("cls");
		gotoXY(5, 5);
		printf("Welcome to Baseball Game!!");
		gotoXY(5, 7);
		printf("1. START");
		gotoXY(5, 8);
		printf("2. EXIT");
		scanf_s("%d", &answer);

	} while (answer != 1 && answer != 2);

	if (answer == 2)
		return 1;

	hThread = _beginthreadex(NULL,
		NULL,
		Animation_threadfunc,
		NULL,
		NULL,
		(unsigned int*)&idThread);

	int v, seta;
	do {

	} while (v != -1 && seta != -1);

	_getch();
	puts("Press any key to exit");
	CloseHandle(hThread);

	return 0;
}

DWORD __stdcall Animation_threadfunc(void)
{

}

void gotoXY(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}