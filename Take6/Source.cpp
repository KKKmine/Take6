//
//                        誰是牛頭王   v1.11
//                                                   by KKKmine  2016/3/31
//上下左右    控制
//Space       選擇/開始遊戲

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include "Game.h"
#include "Player.h"
using namespace std;

#define VER "v1.11"

#define menu_posX 70
#define menu_posY 6

void init_console();
void menu(int* ctrl);
void setting(bool* set);

int main(int argc, char* argv[])
{
	Game game;
	int ctrl[10], point;
	bool set[3];

	init_console();
	menu(ctrl);
	setting(set);

	if (game.init(ctrl, set)){
		system("pause");
	}
	game.reprint();
	point = game.GameLoop();

	return 100 + point;
}


void init_console()
{
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	 
	COORD buf_size = { 150, 46 };
	SMALL_RECT win_rect = { 0, 0, buf_size.X - 1, buf_size.Y - 1 };
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hOuput, &info);
	info.dwSize = buf_size;
	info.srWindow = win_rect;
	SetConsoleScreenBufferSize(hOuput, buf_size);
	SetConsoleWindowInfo(hOuput, true, &win_rect);

	_CONSOLE_CURSOR_INFO  cursor_info;
	GetConsoleCursorInfo(hOuput, &cursor_info);
	cursor_info.bVisible = false;
	SetConsoleCursorInfo(hOuput, &cursor_info);
	SetConsoleTitle(L"誰是牛頭王");

	srand(time(NULL));
}


void menu(int *ctrl)
{
	int kb, select = 0;
	memset(ctrl, EMPTY, sizeof(int)* 10);

	system("cls");
	cout << endl;
	cout << "                                                           _____ _____ _ ___ _____     _____ " << endl;
	cout << "                                                          |_   _|  _  | |  _|   __|   |  ___|" << endl;
	cout << "                                                            | | |  _  |   |_|   __|   |  _  |" << endl;
	cout << "                                                            |_| |_| |_|_|___|_____|   |_____|" << endl;
	for (int i = 0; i < 10; i++)
	{
		gotoxy(menu_posX, menu_posY + 4 * i); cout << "Player : " << i + 1;
		gotoxy(menu_posX + 3 , menu_posY + 1 + 4 * i); cout << ctrl_str[ctrl[i]];
	}
	gotoxy(140, 43);
	cout << VER;

	while (1)
	{
		gotoxy(menu_posX, menu_posY + 1 + 4 * select); cout << "<";
		gotoxy(menu_posX + 10, menu_posY + 1 + 4 * select); cout << ">";
		kb = getch();
		switch (kb)
		{
		case 72: //上
			gotoxy(menu_posX, menu_posY + 1 + 4 * select); cout << " ";
			gotoxy(menu_posX + 10, menu_posY + 1 + 4 * select); cout << " ";
			select += 10 - 1; select %= 10;
			break;
		case 80: //下
			gotoxy(menu_posX, menu_posY + 1 + 4 * select); cout << " ";
			gotoxy(menu_posX + 10, menu_posY + 1 + 4 * select); cout << " ";
			select += 1; select %= 10;
			break;
		case 75: //左
			ctrl[select] += ctrl_types - 1; ctrl[select] %= ctrl_types;
			gotoxy(menu_posX + 3, menu_posY + 1 + 4 * select); cout << ctrl_str[ctrl[select]];
			break;
		case 77: //右
			ctrl[select] += 1; ctrl[select] %= ctrl_types;
			gotoxy(menu_posX + 3, menu_posY + 1 + 4 * select); cout << ctrl_str[ctrl[select]];
			break;
		case 32: //Space
			gotoxy(menu_posX, menu_posY + 1 + 4 * select); cout << " ";
			gotoxy(menu_posX + 10, menu_posY + 1 + 4 * select); cout << " ";
			return;
		}
	}
}

void setting(bool* set)
{
	int kb, select = 0;
	string BOOL_str[2] = { " No ", " Yes" };
	for (int i = 0; i < 10; i++)
	{
		gotoxy(menu_posX, menu_posY + 4 * i); cout << "           ";
		gotoxy(menu_posX + 3, menu_posY + 1 + 4 * i); cout << "     ";
	}
	
	gotoxy(menu_posX , menu_posY + 4 * 0); cout << "Card Delay";
	gotoxy(menu_posX + 3, menu_posY + 1 + 4 * 0); cout << BOOL_str[1]; set[0] = 1;
	gotoxy(menu_posX - 2, menu_posY + 4 * 1); cout << "Perspective Mode";
	gotoxy(menu_posX + 3, menu_posY + 1 + 4 * 1); cout << BOOL_str[0]; set[1] = 0;
	gotoxy(menu_posX - 1, menu_posY + 4 * 2); cout << "Use All Card";
	gotoxy(menu_posX + 3, menu_posY + 1 + 4 * 2); cout << BOOL_str[0]; set[2] = 0;

	while (1)
	{
		gotoxy(menu_posX, menu_posY + 1 + 4 * select); cout << "<";
		gotoxy(menu_posX + 10, menu_posY + 1 + 4 * select); cout << ">";
		kb = getch();
		switch (kb)
		{
		case 72: //上
			gotoxy(menu_posX, menu_posY + 1 + 4 * select); cout << " ";
			gotoxy(menu_posX + 10, menu_posY + 1 + 4 * select); cout << " ";
			select += 3 - 1; select %= 3;
			break;
		case 80: //下
			gotoxy(menu_posX, menu_posY + 1 + 4 * select); cout << " ";
			gotoxy(menu_posX + 10, menu_posY + 1 + 4 * select); cout << " ";
			select += 1; select %= 3;
			break;
		case 75: //左
			if (select != 10){
				set[select] = !set[select];
				gotoxy(menu_posX + 3, menu_posY + 1 + 4 * select); cout << BOOL_str[set[select]];
			}
			break;
		case 77: //右
			if (select != 10){
				set[select] = !set[select];
				gotoxy(menu_posX + 3, menu_posY + 1 + 4 * select); cout << BOOL_str[set[select]];
			}
			break;
		case 32: //Space
			return;
		}
	}
}