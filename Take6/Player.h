#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
using namespace std;

enum ctrl_id{ EMPTY, HUMAN, AI };
const string ctrl_str[] = { "Empty", "Human", " Ai  " };
#define ctrl_types 3

class Player
{
private:
	int id;
	int control;
	int cow_heads;
	vector<int> card;
	int chose_card;

public:
	Player(int i, int ctrl);
	void chooseCard(int id);
	void eat(int num);

	int ai_selectCard(bool *used_card, int *table_data);
	int ai_selectRow(int *table_data);

	friend class Game;
};



#endif
