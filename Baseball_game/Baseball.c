#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <conio.h>
#include <Windows.h>
#include <process.h>
#include <time.h>
#define BODYBLOCK 254
#define TARGET 254
#define ARM 196
#define BAT_1 186
#define BAT_2 61
#define BALL 248
#define FIRST_X_BATTER 10
#define FIRST_Y_BATTER 40
#define FIRST_X_PITCHER 60
#define FIRST_Y_PITCHER 40
#define FIRST_X_FLOATING_BALL (FIRST_X_BATTER + 9)
#define FIRST_Y_FLOATING_BALL (FIRST_Y_BATTER - 2)
#define STANDARD_X_TARGET (FIRST_X_PITCHER + 20)
#define STANDARD_Y_TARGET (FIRST_Y_PITCHER - 20)
#define PI 3.14159265
#define G 10

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
void (*PrintBall[5])(void);

void PrintBatter1(void);
void PrintBatter2(void);
void PrintBatter3(void);
void PrintBatter4(void);
void PrintBatter5(void);
void PrintBatter6(void);
void PrintBatter7(void);
void PrintPitcher1(void);
void PrintPitcher2(void);
void PrintPitcher3(void);
void PrintPitcher4(void);
void PrintPitcher5(void);
void PrintPitcher6(void);
void PrintPitcher7(void);
void PrintBall1(void);
void PrintBall2(void);
void PrintBall3(void);
void PrintBall4(void);
void PrintBall5(void);
void EraseUI(int x, int y);
void PrintFloatingBall(unsigned int x, unsigned int degree,
	unsigned int v, long* floating_ball_x, long* floating_ball_y); 
void PrintHit(void);

void gotoXY(int x, int y);
void setColor(unsigned short color);

unsigned long g_points = 0;
CRITICAL_SECTION g_cs_points;
void PrintPoints(void);
void Plus1Point(void);
void ResetPoint(void);
unsigned long GetPoints(void);

unsigned int g_target_x = 0;
unsigned int g_target_y = 0;
CRITICAL_SECTION g_cs_targets;
void MakeNewTarget(void);
void PrintTarget(void);
unsigned int Get_FIRST_X_TARGET(void);
unsigned int Get_FIRST_Y_TARGET(void);

BOOL g_isEnteringVandDegree = FALSE;
CRITICAL_SECTION g_cs_isEnteringVandDegree;
BOOL GetisEnteringVandDegree(void);
void ChangeisEnteringVandDegree(BOOL value);

void WINAPI ThreadFunc(LPVOID Param);
long double Parabola(unsigned int degree, unsigned int v, unsigned int x);

void Error(const char* message);

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

	PrintBall[0] = PrintBall1;
	PrintBall[1] = PrintBall2;
	PrintBall[2] = PrintBall3;
	PrintBall[3] = PrintBall4;
	PrintBall[4] = PrintBall5;

	system("mode con:cols=500 lines=500");
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

	InitializeCriticalSection(&g_cs_isEnteringVandDegree);
	InitializeCriticalSection(&g_cs_points);
	InitializeCriticalSection(&g_cs_targets);

	MakeNewTarget();
	ResetPoint();

	int ball_x = 0, ball_y = 0;
	int batter_time = 0, pitcher_time = 0,
		throwing_ball_time = 0;
	BOOL swinging_bat = FALSE, throwing_ball = FALSE;
	unsigned int v = 30;
	unsigned int degree = 45;
	do 
	{
		PrintTarget();
		PrintBatter[0]();
		gotoXY(1, 1);
		printf("Enter v : -1 theta : -1 to Exit");

		PrintPoints();

		if ((batter_time >= 2 && batter_time <= 3) && (throwing_ball_time == 4))
		{
			PrintHit();
			DWORD idThread = 0;
			unsigned int* parameters = (unsigned int*)malloc(sizeof(unsigned int) * 2);
			parameters[0] = v;
			parameters[1] = degree;
			HANDLE hThread = _beginthreadex(
				NULL, 0,
				ThreadFunc,
				(LPVOID)parameters,
				0, &idThread);
			CloseHandle(hThread);
		}

		if (swinging_bat == TRUE)
		{
			PrintBatter[batter_time]();
			batter_time++;
			if (batter_time == 7)
			{
				batter_time = 0;
				swinging_bat = FALSE;
			}
		}

		if (throwing_ball == FALSE)
		{
			PrintPitcher[pitcher_time]();
			pitcher_time++;
			if (pitcher_time == 7)
			{
				pitcher_time = 0;
				throwing_ball = TRUE;
			}
		}
		else
			PrintPitcher[0]();

		if (throwing_ball == TRUE)
		{
			PrintBall[throwing_ball_time]();
			throwing_ball_time++;
			if (throwing_ball_time == 5)
			{
				throwing_ball_time = 0;
				throwing_ball = FALSE;
			}
		}

		if (_kbhit())
		{
			char x = _getch();
			if (x == 'c' || x == 'C')
			{
				ChangeisEnteringVandDegree(TRUE);
				Sleep(10);
				gotoXY(50, 45);
				printf("v : ");
				scanf_s("%d", &v);
				getchar();
				gotoXY(60, 45);
				printf("theta : ");
				scanf_s("%d", &degree);
				getchar();
				ChangeisEnteringVandDegree(FALSE);
			}
			else
				swinging_bat = TRUE;
		}
		Sleep(100);
		system("cls");
	} while (v != -1 && degree != -1);

	DeleteCriticalSection(&g_cs_isEnteringVandDegree);
	DeleteCriticalSection(&g_cs_points);
	DeleteCriticalSection(&g_cs_targets);

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

//-------------------------------------------------------

void PrintBall1(void)
{
	gotoXY(FIRST_X_PITCHER - 2, FIRST_Y_PITCHER);
	printf("%c", BALL);
}

void PrintBall2(void)
{
	gotoXY(FIRST_X_PITCHER - 11, FIRST_Y_PITCHER);
	printf("%c", BALL);
}

void PrintBall3(void)
{
	gotoXY(FIRST_X_PITCHER - 22, FIRST_Y_PITCHER);
	printf("%c", BALL);
}

void PrintBall4(void)
{
	gotoXY(FIRST_X_PITCHER - 31, FIRST_Y_PITCHER);
	printf("%c", BALL);
}

void PrintBall5(void)
{
	gotoXY(FIRST_X_PITCHER - 40, FIRST_Y_PITCHER);
	printf("%c", BALL);
}

void EraseUI(int x, int y)
{
	gotoXY(x, y);
	printf(" ");
}

void PrintFloatingBall(unsigned int x, unsigned int degree,
	unsigned int v, long* floating_ball_x, long* floating_ball_y)
{
	unsigned int y = (unsigned int)floor(Parabola(degree, v, x) + 0.5);
	*floating_ball_x = FIRST_X_FLOATING_BALL + x;
	*floating_ball_y = FIRST_Y_FLOATING_BALL - y;
	gotoXY(*floating_ball_x, *floating_ball_y);
	printf("%c", BALL);
}

void PrintHit(void)
{
	gotoXY(FIRST_X_BATTER - 1, FIRST_Y_BATTER - 3);
	printf("Hit!");
}

//-------------------------------------------------------

void gotoXY(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(unsigned short color) 
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

long double Parabola(unsigned int degree, unsigned int v, unsigned int x)
{
	long double theta = degree * (PI / 180);
	long double a = tan(theta) * x;
	long double b1 = (G / 2) * pow(x, 2);
	long double b2 = pow(v, 2) * pow(cos(theta), 2);
	if (b2 == 0)
		return 0;
	long double b = b1 / b2;
	return a - b;
}


void WINAPI ThreadFunc(LPVOID Param)
{
	unsigned int* nParam = (unsigned int*)Param;
	unsigned int v = nParam[0];
	unsigned int degree = nParam[1];
	long present_x = 3;
	long present_y = 3;
	unsigned long out_of_display_time = 0;

	unsigned int time_x = 0;
	while (1)
	{
		PrintFloatingBall(time_x, degree, v, &present_x, &present_y);
		time_x++;

		if (present_x >= FIRST_X_PITCHER + 130 
			|| present_y >= FIRST_Y_PITCHER + 3)
			break;

		if (present_y < 0)
			out_of_display_time++;

		if (out_of_display_time == 20)
			break;

		unsigned int present_target_x = Get_FIRST_X_TARGET();
		unsigned int present_target_y = Get_FIRST_Y_TARGET();
		if ((present_target_x == present_x && present_target_y == present_y)
			|| (present_target_x - 1 == present_x && present_target_y + 1 == present_y)
			|| (present_target_x - 2 == present_x && present_target_y + 2 == present_y))
		{
			gotoXY(present_target_x - 2, present_target_y);
			printf("+1 Point!");
			Plus1Point();
			MakeNewTarget();
			break;
		}

		while (GetisEnteringVandDegree() == TRUE)
			Sleep(10);

		Sleep(60);

		EraseUI(present_x, present_y);
	}
	free(Param);
}

void PrintPoints(void)
{
	gotoXY(1, 10);
	printf("Points : %ld", GetPoints());
}

unsigned long GetPoints(void)
{
	unsigned long result = 0;
	EnterCriticalSection(&g_cs_points);
	result = g_points;
	LeaveCriticalSection(&g_cs_points);
	return result;
}

void Plus1Point(void)
{
	EnterCriticalSection(&g_cs_points);
	g_points++;
	LeaveCriticalSection(&g_cs_points);
}

void ResetPoint(void)
{
	EnterCriticalSection(&g_cs_points);
	g_points = 0;
	LeaveCriticalSection(&g_cs_points);
}

void MakeNewTarget(void)
{
	EnterCriticalSection(&g_cs_targets);
	srand((unsigned int)time(NULL));
	unsigned int ran_x = rand() % 15;
	srand((unsigned int)time(NULL));
	unsigned int ran_y = rand() % 15;
	srand((unsigned int)time(NULL));
	if (rand() % 2 == 1)
	{
		srand((unsigned int)time(NULL));
		if (rand() % 2 == 1)
		{
			g_target_x = STANDARD_X_TARGET + ran_x;
			g_target_y = STANDARD_Y_TARGET + ran_y;
		}
		else
		{
			g_target_x = STANDARD_X_TARGET + ran_x;
			g_target_y = STANDARD_Y_TARGET - ran_y;
		}
	}
	else
	{
		srand((unsigned int)time(NULL));
		if (rand() % 2 == 1)
		{
			g_target_x = STANDARD_X_TARGET - ran_x;
			g_target_y = STANDARD_Y_TARGET + ran_y;
		}
		else
		{
			g_target_x = STANDARD_X_TARGET - ran_x;
			g_target_y = STANDARD_Y_TARGET - ran_y;
		}
	}
	LeaveCriticalSection(&g_cs_targets);
}

void PrintTarget(void)
{
	EnterCriticalSection(&g_cs_targets);
	gotoXY(g_target_x, g_target_y);
	printf("%c", TARGET);
	gotoXY(g_target_x - 1, g_target_y + 1);
	printf("%c", TARGET);
	gotoXY(g_target_x - 2, g_target_y + 2);
	printf("%c", TARGET);
	LeaveCriticalSection(&g_cs_targets);
}

unsigned int Get_FIRST_X_TARGET(void)
{
	unsigned int result = 0;
	EnterCriticalSection(&g_cs_targets);
	result = g_target_x;
	LeaveCriticalSection(&g_cs_targets);
	return result;
}

unsigned int Get_FIRST_Y_TARGET(void)
{
	unsigned int result = 0;
	EnterCriticalSection(&g_cs_targets);
	result = g_target_y;
	LeaveCriticalSection(&g_cs_targets);
	return result;
}

BOOL GetisEnteringVandDegree(void)
{
	BOOL result = 0;
	EnterCriticalSection(&g_cs_isEnteringVandDegree);
	result = g_isEnteringVandDegree;
	LeaveCriticalSection(&g_cs_isEnteringVandDegree);
	return result;
}

void ChangeisEnteringVandDegree(BOOL value)
{
	EnterCriticalSection(&g_cs_isEnteringVandDegree);
	g_isEnteringVandDegree = value;
	LeaveCriticalSection(&g_cs_isEnteringVandDegree);
}

void Error(const char* message)
{
	printf("\n\n%s\n\n", message);
	_getch();
	exit(1);
}