#include "Player.h"


Player::Player(int i, int ctrl)
{
	id = i;
	control = ctrl;
	card.resize(10);
	cow_heads = 0;
	chose_card = -1;
}


void Player::chooseCard(int id)
{
	if (id < card.size()){
		chose_card = card[id];
		card.erase(card.begin() + id);
	}
}

void Player::eat(int num)
{
	cow_heads += num;
}

int Player::ai_selectCard(bool *used_card,int *table_data) //table_data 0~3Ю狠礟id 4~7êrow礟计秖 8~11êrow羆繷 12產计
{
	int space_card[10]; //讽场常逼 临Τぶ
	int put_row[10]; //row
	int select = -1;
	vector<int> noteat_select, mayeat_select;

	for (int i = 0; i < card.size(); i++)
	{
		pair<int, int> min_diff = make_pair(-1, 9999);
		int diff;

		//т籔计程
		for (int j = 0; j < 4; j++){
			diff = card[i] - table_data[j];
			if (diff < min_diff.second && diff > 0){
				min_diff.first = j;
				min_diff.second = diff;
			}
		}

		//тrow
		if (min_diff.first == -1) //ゑ–逼常
		{
			put_row[i] = 0; //т程繷row
			for (int j = 0; j < 4; j++){
				if (table_data[8 + j] < table_data[8 + put_row[i]]) {
					put_row[i] = j;
				}
			}
		}
		else if (table_data[4 + min_diff.first] == 5) //禬筁5眎璶礟
		{
			put_row[i] = min_diff.first;
		}
		else //ぃノ礟
		{
			put_row[i] = min_diff.first;
			//т禬碭眎逼
			if (min_diff.first >= 0){
				space_card[i] = 5 - table_data[4 + min_diff.first];
				for (int j = table_data[min_diff.first] + 1, s = 0; j < card[i] && s < table_data[12] - 1; j++){
					if (!used_card[j]){
						s++;
						space_card[i]--;
					}
				}
				if (space_card[i] > 0){
					noteat_select.push_back(i);
				}
				else{
					mayeat_select.push_back(i);
				}
			}
		}
	}

	if (noteat_select.size() > 0) //тゲぃ礟ㄤい眎礟
	{
		select = noteat_select[rand() % noteat_select.size()];
	}
	else if (mayeat_select.size() > 0) //т礟程ぶ眎礟
	{
		for (int i = 0; i < mayeat_select.size(); i++){
			for (int j = i + 1; j < mayeat_select.size(); j++){
				if (table_data[4 + put_row[mayeat_select[i]]] > table_data[4 + put_row[mayeat_select[j]]]){
					swap(mayeat_select[i], mayeat_select[j]);
				}
			}
		}
		select = mayeat_select[0];
	}
	else if (select == -1) //┮Τ礟常穦 т程ぶ繷
	{
		for (int i = 0; i < card.size(); i++){
			if (select == -1 || table_data[8 + put_row[i]] < table_data[8 + put_row[select]]) {
				select = i;
			}
		}
	}

	if (select >= card.size() || select < 0){
		select = rand() % card.size();
	}
	return select;
}

int Player::ai_selectRow(int *table_data){
	int min_row = 0;
	for (int i = 0; i < 4; i++){
		if (table_data[8 + i] < table_data[8 + min_row]){
			min_row = i;
		}
	}
	return min_row;
}