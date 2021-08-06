#pragma once
//parameters of monster's action card
#include <string>
using namespace std;
typedef struct MonsterActionCard
{
	int number;
	int agility;
	string move;
	int shield = 0;
	bool isShield = false;
	int attack = 0;
	bool isAttack = false;
	int range = 0;
	bool isRange = false;
	int heal = 0;
	bool isHeal = false;
	bool reuse;
}MonsterActCard;
