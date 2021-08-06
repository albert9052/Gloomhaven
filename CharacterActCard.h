#pragma once
#include <vector>
using namespace std;
//parameters of character's action card
typedef struct CharacterActionCard
{
	int number;
	int agility;
	vector<int>topAttack;
	vector<int>topHeal;
	vector<int>topShield;
	vector<int>topMove;
	int topRange;
	vector<int>botAttack;
	vector<int>botHeal;
	vector<int>botShield;
	vector<int>botMove;
	int botRange;
}CharacterActCard;