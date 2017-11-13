#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include "Player.h"
using namespace std;

class Game
{
private:
	int round;
	int player_num;
	vector<vector<int> > table;
	Player* players[10];
	bool used_card[104];
	bool isOneHuman;

	bool isPerspective;
	bool isDelay;

public:
	bool init(int *ctrl, bool *setting);
	void shuffler(int type);
	int GameLoop();

	int selectCard(int item);
	int selectRow();
	void reprint();
	void cleanMessage();
	void showCard(pair<int, int> *pos, int id);
	void showPlayerCard(int p); //¤âµP
	void showTableCard(int x, int y); //µP®à
	void showScoreBoard(int p); //°O¤ÀªO
};


inline void gotoxy(int xpos, int ypos)
{
	static HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD scrn;
	scrn.X = xpos;
	scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}


#endif
