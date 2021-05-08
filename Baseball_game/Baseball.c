#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <conio.h>
#include <Windows.h>
#include <process.h>
#define BODYBLOCK 254
#define ARM 196
#define BAT_1 186
#define BAT_2 61
#define FIRST_X_BATTER 10
#define FIRST_Y_BATTER 30
#define FIRST_X_PITCHER 60
#define FIRST_Y_PITCHER 30

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

void (*PrintBatter[7])(void);
void (*PrintPitcher[7])(void);

void PrintBatter1(int frame);
void PrintBatter2(int frame);
void PrintBatter3(int frame);
void PrintBatter4(int frame);
void PrintBatter5(int frame);
void PrintBatter6(int frame);
void PrintBatter7(int frame);
void PrintPitcher1(int frame);
void PrintPitcher2(int frame);
void PrintPitcher3(int frame);
void PrintPitcher4(int frame);
void PrintPitcher5(int frame);
void PrintPitcher6(int frame);
void PrintPitcher7(int frame);

void gotoXY(int x, int y);
void setColor(unsigned short color);

int main(void)
{
	SetConsoleOutputCP(437);

	PrintBatter[0] = PrintBatter1;
	PrintBatter[1] = PrintBatter2;
	PrintBatter[2] = PrintBatter3;
	PrintBatter[3] = PrintBatter4;
	PrintBatter[4] = PrintBatter5;
	PrintBatter[5] = PrintBatter6;
	PrintBatter[6] = PrintBatter7;

	PrintPitcher[0] = PrintPitcher1;
	PrintPitcher[1] = PrintPitcher2;
	PrintPitcher[2] = PrintPitcher3;
	PrintPitcher[3] = PrintPitcher4;
	PrintPitcher[4] = PrintPitcher5;
	PrintPitcher[5] = PrintPitcher6;
	PrintPitcher[6] = PrintPitcher7;

	system("mode con:cols=500 lines=500");
	system("color F");
	SetConsoleTitle(_T("Baseball_Game"));
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
	fflush(stdin);
	if (answer == 2)
		return 1;
	system("cls");

	int ball_x = 0, ball_y = 0;
	int v = 0, theta = 0, batter_time = 0, pitcher_time = 0;
	BOOL hit_flag = FALSE;
	do {
		PrintBatter[0]();
		gotoXY(1, 1);
		printf("Enter v : -1 theta : -1 to Exit");

		if (hit_flag == TRUE)
		{
			PrintBatter[batter_time]();
			batter_time++;
			if (batter_time == 7)
			{
				batter_time = 0;
				hit_flag = FALSE;
			}
		}

		PrintPitcher[pitcher_time]();
		pitcher_time++;
		if (pitcher_time == 7)
			pitcher_time = 0;

		if (_kbhit())
		{
			char x = _getch();
			if (x == 'c' || x == 'C')
			{
				gotoXY(50, 50);
				printf("v : ");
				scanf_s("%d", &v);
				getchar();
				gotoXY(60, 50);
				printf("theta : ");
				scanf_s("%d", &theta);
				getchar();
			}
			else
				hit_flag = TRUE;
		}
		Sleep(200);
		system("cls");
	} while (v != -1 && theta != -1);


	puts("Press any key to exit");
	_getch();

	return 0;
}

void PrintBatter1(void)
{
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER);
	printf("%c %c ", BAT_1, BODYBLOCK);
	gotoXY(FIRST_X_BATTER + 1, FIRST_Y_BATTER);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 1);
	printf("%c%c ", BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

void PrintBatter2(void)
{
	gotoXY(FIRST_X_BATTER - 1, FIRST_Y_BATTER);
	printf("%c  %c ", BAT_1, BODYBLOCK);
	gotoXY(FIRST_X_BATTER - 1, FIRST_Y_BATTER + 1);
	printf("%c%c%c ", BODYBLOCK, ARM, BODYBLOCK);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

void PrintBatter3(void)
{
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER);
	printf("  %c ", BODYBLOCK);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 1);
	printf("%c %c ", BAT_2, BODYBLOCK);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

void PrintBatter4(void)
{
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER);
	printf("  %c ", BODYBLOCK);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 1);
	printf("  %c %c", BODYBLOCK, BAT_2);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

void PrintBatter5(void)
{
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER - 1);
	printf("     %c", BAT_1);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER);
	printf("  %c%c", BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 1);
	printf("  %c ", BODYBLOCK);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

void PrintBatter6(void)
{
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER);
	printf("  %c%c", BODYBLOCK, BAT_1);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 1);
	printf("  %c%c", BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

void PrintBatter7(void)
{
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER);
	printf("%c %c ", BAT_1, BODYBLOCK);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 1);
	printf("%c%c ", BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_BATTER, FIRST_Y_BATTER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

//-------------------------------------------------------

void PrintPitcher1(void)
{
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER);
	printf("  %c ", BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 1);
	printf("%c%c%c", BODYBLOCK, BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

void PrintPitcher2(void)
{
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER);
	printf("  %c ", BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 1);
	printf("  %c%c", BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

void PrintPitcher3(void)
{
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER);
	printf("  %c%c", BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 1);
	printf("  %c  ", BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

void PrintPitcher4(void)
{
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER);
	printf("  %c%c", BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 1);
	printf("  %c  ", BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

void PrintPitcher5(void)
{
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER);
	printf("  %c%c", BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 1);
	printf("  %c%c", BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 2);
	printf("    %c", BODYBLOCK);
}

void PrintPitcher6(void)
{
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER);
	printf("%c%c ", BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 1);
	printf("  %c  ", BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 2);
	printf("%c   %c", BODYBLOCK, BODYBLOCK);
}

void PrintPitcher7(void)
{
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER);
	printf("  %c ", BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 1);
	printf("%c%c%c", BODYBLOCK, BODYBLOCK, BODYBLOCK);
	gotoXY(FIRST_X_PITCHER, FIRST_Y_PITCHER + 2);
	printf("%c  %c", BODYBLOCK, BODYBLOCK);
}

void gotoXY(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(unsigned short color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}