#pragma once

#include <string>
#include <vector>
#include "MonsterActCard.h"
using namespace std;
//parameters of monster's data
typedef struct MonsterData
{
	string name;
	int normalLargestHP;
	int normalBaseATK;
	int normalBaseRange;
	int eliteLargestHP;
	int eliteBaseATK;
	int eliteBaseRange;
	vector<MonsterActCard>actions;
}MonsterData;