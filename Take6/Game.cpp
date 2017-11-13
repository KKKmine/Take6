#include "Game.h"

#define SLEEP_TIME 500*isDelay

//card_id
#define cover_card -1
#define empty_card 0
#define non_card 9999

//card_pos
inline pair<int, int> player_pos(int x){
	return make_pair(x * 12 + 2, 2);
}
inline pair<int, int> table_pos(int x,int y){
	return make_pair(x * 12 + 2, y * 8 + 12);
}
inline pair<int, int> choose_pos(int num){
	return make_pair(74 + (num % 3) * 12, 12 + (num / 3) * 8);
}

inline int getCowHead(int id)
{
	int cowhead = 0;
	if (id % 10 == 5){
		cowhead = 2;
	}
	if (id % 10 == 0){
		cowhead = 3;
	}
	if (id % 11 == 0){
		cowhead += 5;
	}
	if (cowhead == 0){
		cowhead = 1;
	}
	return cowhead;
}

bool Game::init(int *ctrl, bool *setting)
{
	int locol_human = 0;

	vector<int> v;
	table.clear();
	table.push_back(v);
	table.push_back(v);
	table.push_back(v);
	table.push_back(v);
	memset(used_card, false, sizeof(bool)* 104);

	player_num = 0;
	round = 1;
	isDelay = setting[0];
	isPerspective = setting[1];

	for (int i = 0; i < 10; i++)
	{
		if (ctrl[i] == EMPTY){
			players[i] = NULL;
		}
		else if (ctrl[i] == HUMAN){
			players[i] = new Player(i, ctrl[i]);
			player_num++;
			locol_human++;
		}
		else if (ctrl[i] == AI){
			players[i] = new Player(i, ctrl[i]);
			player_num++;
		}
		else{
			cout << "Init Fail" << endl;
			return 1;
		}
	}
	if (locol_human == 1){
		isOneHuman == true;
	}
	else{
		isOneHuman = false;
	}

	shuffler(setting[2]);
	return 0;
}

void Game::shuffler(int type)
{
	vector<int> card_id;

	if (type == 0){
		card_id.resize(player_num * 10 + 4);
	}
	else{
		card_id.resize(104);
	}

	for (int i = 0; i < card_id.size(); i++){
		card_id[i] = i + 1;
	}
	random_shuffle(card_id.begin(), card_id.end());
	for (int i = 0, g = 0; i < 10; i++){
		if (players[i] != NULL){
			players[i]->card.resize(10);
			for (int j = 0; j < 10 && g < card_id.size(); j++, g++){
				players[i]->card[j] = card_id.at(g);
			}
			sort(players[i]->card.begin(), players[i]->card.end());
		}
	}
	table[0].push_back(card_id[card_id.size() - 1]);
	table[1].push_back(card_id[card_id.size() - 2]);
	table[2].push_back(card_id[card_id.size() - 3]);
	table[3].push_back(card_id[card_id.size() - 4]);
}

int Game::selectCard(int item)
{
	int kb, select = 0;
	gotoxy(7, 10);
	cout << "△";
	while (1)
	{
		kb = getch();
		switch (kb)
		{
		case 75: //左
			gotoxy(select * 12 + 7, 10);
			cout << "  ";
			select += item - 1;
			select %= item;
			gotoxy(select * 12 + 7, 10);
			cout << "△";
			break;
		case 77: //右
			gotoxy(select * 12 + 7, 10);
			cout << "  ";
			select += 1;
			select %= item;
			gotoxy(select * 12 + 7, 10);
			cout << "△";
			break;
		case 32: //Space
			gotoxy(select * 12 + 7, 10);
			cout << "  ";
			return select;
		}
	}
}

int Game::selectRow()
{
	int kb, select = 0;
	gotoxy(62, 15);
	cout << "<<";
	while (1)
	{
		kb = getch();
		switch (kb)
		{
		case 72: //上
			gotoxy(62, select * 8 + 15);
			cout << "  ";
			select += 3;
			select %= 4;
			gotoxy(62, select * 8 + 15);
			cout << "<<";
			break;
		case 80: //下
			gotoxy(62, select * 8 + 15);
			cout << "  ";
			select += 1;
			select %= 4;
			gotoxy(62, select * 8 + 15);
			cout << "<<";
			break;
		case 32: //Space
			gotoxy(62, select * 8 + 15);
			cout << "  ";
			return select;
		}
	}
}

void Game::reprint()
{
	system("cls");
	//手牌
	showPlayerCard(-1);
	//牌桌
	for (int j = 0; j < 4; j++){
		for (int i = 0; i < 5; i++){
			showTableCard(i, j); 
		}
	}
	//記分板
	for (int i = 0; i < 10; i++){
		showScoreBoard(i);
	}
	if (isPerspective){
		gotoxy(130, 43);
		cout << " Perspective Mode";
	}
}

void Game::showCard(pair<int, int> *pos, int id)
{
	if (id == non_card)
	{
		for (int i = 0; i < 8; i++){
			gotoxy(pos->first, pos->second + i);
			cout << "            ";
		}
	}
	else if (id == cover_card)
	{
		gotoxy(pos->first, pos->second);
		cout << "┌────┐ ";
		for (int i = 0; i<3; i++)
		{
			gotoxy(pos->first, pos->second + 2 * i + 1);
			cout << "│\\/\\/\\/\\/│ ";
			gotoxy(pos->first, pos->second + 2 * i + 2);
			cout << "│/\\/\\/\\/\\│ ";
		}
		gotoxy(pos->first, pos->second + 7);
		cout << "└────┘ ";
	}
	else
	{
		gotoxy(pos->first, pos->second);
		cout << "┌────┐";
		switch (getCowHead(id))
		{
		case 1:
			gotoxy(pos->first, pos->second + 1);
			cout << "│   牛   │";
			gotoxy(pos->first, pos->second + 2);
			cout << "│        │";
			break;
		case 2:
			gotoxy(pos->first, pos->second + 1);
			cout << "│  牛牛  │";
			gotoxy(pos->first, pos->second + 2);
			cout << "│        │";
			break;
		case 3:
			gotoxy(pos->first, pos->second + 1);
			cout << "│ 牛牛牛 │";
			gotoxy(pos->first, pos->second + 2);
			cout << "│        │";
			break;
		case 5:
			gotoxy(pos->first, pos->second + 1);
			cout << "│ 牛牛牛 │";
			gotoxy(pos->first, pos->second + 2);
			cout << "│  牛牛  │";
			break;
		case 7:
			gotoxy(pos->first, pos->second + 1);
			cout << "│牛牛牛牛│";
			gotoxy(pos->first, pos->second + 2);
			cout << "│ 牛牛牛 │";
			break;
		default:
			gotoxy(pos->first, pos->second + 1);
			cout << "│┌    ┐│";
			gotoxy(pos->first, pos->second + 2);
			cout << "│        │";
			break;
		}
		gotoxy(pos->first, pos->second + 3);
		cout << "│        │";
		gotoxy(pos->first, pos->second + 4);
		cout << "│        │";
		if (id < 10 && id > 0) {
			gotoxy(pos->first, pos->second + 5);
			cout << "│    " << id << "   │";
			gotoxy(pos->first, pos->second + 6);
			cout << "│        │";
		}
		else if (id < 100 && id > 0){
			gotoxy(pos->first, pos->second + 5);
			cout << "│   " << id << "   │";
			gotoxy(pos->first, pos->second + 6);
			cout << "│        │";
		}
		else if (id <= 104 && id > 0) {
			gotoxy(pos->first, pos->second + 5);
			cout << "│  " << id << "   │";
			gotoxy(pos->first, pos->second + 6);
			cout << "│        │";
		}
		else{
			gotoxy(pos->first, pos->second + 5);
			cout << "│        │";
			gotoxy(pos->first, pos->second + 6);
			cout << "│└    ┘│";
		}
		gotoxy(pos->first, pos->second + 7);
		cout << "└────┘";
	}
}

void Game::showPlayerCard(int p)
{
	int i;

	if (p >= 0 && p <= 10){
		for (i = 0; i < players[p]->card.size(); i++){
			showCard(&player_pos(i), players[p]->card[i]);
		}
	}
	else{
		for (i = 0; i < 11 - round; i++){
			showCard(&player_pos(i), cover_card);
		}
	}
	if (i < 10){
		showCard(&player_pos(i), non_card);
	}
}

void Game::showTableCard(int x,int y)
{
	if (x < table[y].size()){
		showCard(&table_pos(x, y), table[y][x]);
	}
	else{
		showCard(&table_pos(x, y), empty_card);
	}
	return;
		
}

void Game::showScoreBoard(int p)
{
	gotoxy(130, 4 * p + 2);
	cout << "Player " << p + 1 << " : ";
	if (players[p] != NULL) {
		cout << ctrl_str[players[p]->control];
		gotoxy(130, 4 * p + 3);
		cout << "Cow Heads : " << players[p]->cow_heads;
	}
	else{
		cout << ctrl_str[EMPTY];
	}
}

void Game::cleanMessage()
{
	gotoxy(2, 0);
	for (int i = 0; i < 10; i++){
		cout << "          ";
	}
}

int Game::GameLoop()
{
	Player* order[10];
	int order_at;
	int table_data[13];

	if (player_num == 0){
		return -102;
	}
	for (round = 1; round <= 10; round++)
	{
		gotoxy(130, 0);
		cout << "Round : " << round;
		//選牌
		for (int i = 0; i < 4; i++){
			table_data[i] = table[i].back();
			table_data[4 + i] = table[i].size();
			table_data[8 + i] = 0;
			for (int j = 0; j < table[i].size(); j++){
				table_data[8 + i] += getCowHead(table[i][j]);
			}
		}
		table_data[12] = player_num;

		order_at = 0;
		for (int i = 0; i < 10; i++)
		{
			cleanMessage();
			if (players[i] != NULL)
			{
				order[order_at] = players[i]; 
				order_at++;

				if (players[i]->control == HUMAN)
				{
					if (!isOneHuman){
						gotoxy(2, 0);
						cout << "輪到 Player " << i + 1 << " 選牌 請其他人閉眼 否則詛咒你魯四年   ";
						system("pause");
						cleanMessage();
						gotoxy(2, 0);
						cout << "輪到 Player " << i + 1 << " 選牌";
						showPlayerCard(i);
						players[i]->chooseCard(selectCard(players[i]->card.size()));
						showPlayerCard(-1);
					}
					else{
						if (round == 1 || isPerspective){
							showPlayerCard(i);
						}
						gotoxy(2, 0);
						cout << "輪到你選牌";
						players[i]->chooseCard(selectCard(players[i]->card.size()));
						if (!isPerspective){
							showPlayerCard(i);
						}
					}
				}
				else if (players[i]->control == AI){
					if (isPerspective){
						showPlayerCard(i);
						gotoxy(2, 0);
						cout << "輪到 Player " << i + 1 << " 選牌";
					}

					int ai_choose_pos = players[i]->ai_selectCard(used_card, table_data);
					players[i]->chooseCard(ai_choose_pos); //ai選牌

					if (isPerspective){
						gotoxy(ai_choose_pos * 12 + 7, 10);
						cout << "△";
						Sleep(SLEEP_TIME);
						gotoxy(ai_choose_pos * 12 + 7, 10);
						cout << " ";
					}
				}
			}
		}

		if (!isOneHuman){
			showCard(&player_pos(10 - round), non_card);
		}
		cleanMessage();
		gotoxy(2, 0);
		//排序
		for (int i = 0; i < player_num; i++){
			for (int j = i + 1; j < player_num; j++){
				if (order[i]->chose_card > order[j]->chose_card){
					swap(order[i], order[j]);
				}
			}
		}

		//顯示排序
		for (int i = 0; i < player_num; i++){
			showCard(&choose_pos(i), order[i]->chose_card);
			gotoxy(choose_pos(i).first + 2 , choose_pos(i).second);
			cout << "Player " << order[i]->id + 1;
		}

		//丟牌
		for (int i = 0; i < player_num; i++)
		{
			int diff, heads = 0;
			pair<int, int> min_diff = make_pair(-1, 9999);


			for (int j = 0; j < 4; j++) //找與數字最近的列
			{
				diff = order[i]->chose_card - table[j].back();
				if (min_diff.second > diff && diff>0){
					min_diff.first = j;
					min_diff.second = diff;
				}
			}
			if (min_diff.second > 0 && min_diff.second < 104 && table[min_diff.first].size()<5) //放牌
			{
				table[min_diff.first].push_back(order[i]->chose_card);
				used_card[order[i]->chose_card] = true;
				order[i]->chose_card = 0;
				showTableCard(table[min_diff.first].size() - 1, min_diff.first);

				if (isDelay){
					cleanMessage();
					gotoxy(2, 0);
					cout << "Player " << order[i]->id + 1 << " 丟牌到第 " << min_diff.first + 1 << " 排";
				}
				Sleep(SLEEP_TIME);
			}
			else //吃牌
			{
				if (min_diff.first == -1){
					if (order[i]->control == HUMAN){
						gotoxy(2, 0);
						cout << "Player " << order[i]->id + 1 << " 請選擇一列吃牌";
						min_diff.first = selectRow();
					}
					else if (order[i]->control == AI){
						min_diff.first = players[order[i]->id]->ai_selectRow(table_data); //ai選列
					}
				}
				for (int j = 0; j < table[min_diff.first].size(); j++){
					heads += getCowHead(table[min_diff.first][j]);
				}
				order[i]->eat(heads);
				table[min_diff.first].clear();
				table[min_diff.first].push_back(order[i]->chose_card);
				order[i]->chose_card = -1;

				showScoreBoard(order[i]->id);
				for (int j = 0; j < 5; j++){
					showTableCard(j, min_diff.first);
				}

				if (isDelay){
					gotoxy(2, 0);
					cout << "Player " << order[i]->id + 1 << " 丟牌到第 " << min_diff.first + 1 << " 排 吃了 " << heads << " 個牛頭";
				}
				Sleep(SLEEP_TIME * 3);
			}
			showCard(&choose_pos(i), non_card);
		}
	}

	//遊戲結束
	gotoxy(2, 0);
	cout << "遊戲結束   ";
	system("pause");
	system("cls");

	gotoxy(70, 2);
	cout << "-----Score-----";
	if (isPerspective){
		gotoxy(70, 3);
		cout << "Perspective Mode";
	}
	order_at = 0;
	for (int i = 0; i < 10; i++){
		if (players[i] != NULL){
			order[order_at] = players[i];
			order_at++;
		}
	}
	//排名
	for (int i = 0; i < player_num; i++){
		for (int j = i + 1; j < player_num; j++){
			if (order[i]->cow_heads > order[j]->cow_heads){
				swap(order[i], order[j]);
			}
		}
	}
	//公布成績
	int return_point = 0;
	for (int i = 0, no = 1; i < player_num; i++){
		if (i>0){
			if (order[i - 1]->cow_heads != order[i]->cow_heads){
				no++;
			}
		}
		gotoxy(70, 4 * i + 5); cout << "No. " << no << endl;
		gotoxy(70, 4 * i + 6); cout << "Player " << order[i]->id + 1 << " : " << ctrl_str[order[i]->control] << endl;
		gotoxy(70, 4 * i + 7); cout << "Cow Heads : " << order[i]->cow_heads << endl << endl;
		if (order[i]->control == HUMAN && return_point == 0){
			return_point = player_num - no;
		}
	}
	
	system("pause");
	return return_point;
}
