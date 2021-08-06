#include "ReadCharacterData.h"
#include "CharacterData.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
vector<CharacterData> readCharacterData(string fileName)
{
	vector<CharacterData> characters;
	//input file
	ifstream characterFile;
	characterFile.open(fileName);
	if (!characterFile.is_open())
	{
		cout << "Error: " << endl;
		cout << "Can't open " << fileName << endl;
		exit(1);
	}
	//parameter creating
	int number;
	characterFile >> number;
	characterFile.clear();
	//while there are characters for input
	string name;
	while (characterFile >> name)
	{
		//enter parameters
		int characterHP, characterCardNumber;
		characterFile >> characterHP >> characterCardNumber;
		//temporary character data for later input
		CharacterData temp;
		temp.name = name;
		temp.initCards = characterCardNumber;
		temp.largestHP = characterHP;
		int cardAmount;
		characterFile >> cardAmount;
		temp.HP = characterHP;
		//input card data
		for (int i = 0; i < cardAmount; i++)
		{
			int cardNum, cardAgility;
			characterFile >> cardNum >> cardAgility;
			//temporary card for later pushback
			CharacterActCard card;
			card.number = cardNum;
			card.agility = cardAgility;
			characterFile.ignore();
			string content;
			getline(characterFile, content);
			stringstream ss;
			ss.clear();
			ss << content;
			string action;

			bool setRange = false;
			//top phase
			while (ss >> action)
			{
				if (action == "attack")
				{
					int cardAttack;
					ss >> cardAttack;
					card.topAttack.push_back(cardAttack);
				}
				if (action == "shield")
				{
					int cardShield;
					ss >> cardShield;
					card.topShield.push_back(cardShield);
				}
				if (action == "move")
				{
					int cardMove;
					ss >> cardMove;
					card.topMove.push_back(cardMove);
				}
				if (action == "heal")
				{
					int cardHeal;
					ss >> cardHeal;
					card.topHeal.push_back(cardHeal);
				}
				if (action == "range")
				{
					setRange = true;
					int cardRange;
					ss >> cardRange;
					card.topRange = cardRange;
				}
				if (action == "-")
				{
					if (setRange == false)
						card.topRange = 0;
					break;
				}
			}

			setRange = false;
			//bottom phase
			while (ss >> action)
			{
				if (action == "attack")
				{
					int cardAttack;
					ss >> cardAttack;
					card.botAttack.push_back(cardAttack);
				}
				if (action == "shield")
				{
					int cardShield;
					ss >> cardShield;
					card.botShield.push_back(cardShield);
				}
				if (action == "move")
				{
					int cardMove;
					ss >> cardMove;
					card.botMove.push_back(cardMove);
				}
				if (action == "heal")
				{
					int cardHeal;
					ss >> cardHeal;
					card.botHeal.push_back(cardHeal);
				}
				if (action == "range")
				{
					setRange = true;
					int cardRange;
					ss >> cardRange;
					card.botRange = cardRange;
				}
			}
			//pushback card
			if (setRange == false)
				card.botRange = 0;
			temp.actions.push_back(card);
		}
		//sort card with number
		sort(temp.actions.begin(), temp.actions.end(), compareCharacterCard);
		//pushback character data
		characters.push_back(temp);
	}
	return characters;
}

/*compareCharacterCard()*/
	// Intent: To compare stats for implementation of sort function in readCharacterData().
	// Pre: Two characterActCards for comparision.
	// Post: The function return true or false for sorting.
bool compareCharacterCard(CharacterActCard a, CharacterActCard b)
{
	return a.number < b.number;
}
