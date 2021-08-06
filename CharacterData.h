#pragma once
#include <string>
#include <vector>
#include "CharacterActCard.h"
//parameters of character's data
typedef struct CharacterData
{
	string name;
	int largestHP;
	int initCards;
	int HP;
	vector<CharacterActCard>actions;
}CharacterData;