#include "ReadMonsterData.h"
#include <iostream>

// Intent: To read file containing data of monster and save into game memory.
// Pre: The file must be in regulated format.
// Post: The function doesn't return anything but saves data into memory in needed format(structure). 
vector<MonsterData> readMonsterData(string fileName)
{
	//bool for checking whether EOF is read
	bool end = false;
	//input file
	ifstream monsterFile;
	monsterFile.open(fileName);
	if (!monsterFile.is_open())
	{
		cout << "Error: " << endl;
		cout << "Can't open " << fileName << endl;
		exit(1);
	}
	//input parameters
	vector<MonsterData> monsters;
	int number;
	monsterFile >> number;
	string name;
	monsterFile >> name;
	//while there are still monsters to read in
	for (int i = 0; i < number; i++)
	{
		//temporary monster data for later pushback
		MonsterData temp;
		//parameter inputting
		temp.name = name;
		int nHP, nATK, nRange, eHP, eATK, eRange;
		monsterFile >> nHP >> nATK >> nRange >> eHP >> eATK >> eRange;
		temp.normalLargestHP = nHP;
		temp.normalBaseATK = nATK;
		temp.normalBaseRange = nRange;
		temp.eliteLargestHP = eHP;
		temp.eliteBaseATK = eATK;
		temp.eliteBaseRange = eRange;

		//for checking of next monster
		string cardOwner;
		monsterFile >> cardOwner;
		//if card belongs to previous name
		while (cardOwner == name)
		{
			//input parameters
			int number, agility;
			monsterFile >> number >> agility;
			//temporary card for later pushback
			MonsterActCard card;
			card.number = number;
			card.agility = agility;
			//ignore one character for later getline
			monsterFile.ignore();
			string content;
			getline(monsterFile, content);
			//throw into stringstream, determine actions
			stringstream ss;
			ss.clear();
			ss << content;
			string action;
			//while not end of line
			while (ss >> action)
			{
				if (action == "move")
				{
					string cardMove;
					ss >> cardMove;
					card.move = cardMove;
				}
				if (action == "attack")
				{
					int cardAttack;
					ss >> cardAttack;
					card.attack = cardAttack;
					card.isAttack = true;
				}
				if (action == "range")
				{
					int cardRange;
					ss >> cardRange;
					card.range = cardRange;
					card.isRange = true;
				}
				if (action == "shield")
				{
					int cardShield;
					ss >> cardShield;
					card.shield = cardShield;
					card.isShield = true;
				}
				if (action == "heal")
				{
					int cardHeal;
					ss >> cardHeal;
					card.heal = cardHeal;
					card.isHeal = true;
				}
				if (action == "r")
				{
					card.reuse = true;
				}
				if (action == "d")
				{
					card.reuse = false;
				}
			}
			//pushback action card
			temp.actions.push_back(card);
			//check if end of file, if true break
			if (!(monsterFile >> cardOwner)) {
				end = true;
				break;
			}
		}
		//set new name equal to previous input
		if (end != true) {
			name = cardOwner;
		}
		sort(temp.actions.begin(), temp.actions.end(), compareMonsterCard);
		monsters.push_back(temp);
	}
	return monsters;
}

/*compareMonsterCard()*/
	// Intent: To compare stats for implementation of sort function in readMonsterData().
	// Pre: Two monsterActCards for comparision.
	// Post: The function return true or false for sorting.
bool compareMonsterCard(MonsterActCard a, MonsterActCard b)
{
	return a.number < b.number;
}