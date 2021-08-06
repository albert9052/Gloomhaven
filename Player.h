#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <io.h>
#include <algorithm>
#include "Creature.h"
#include "ReadCharacterData.h"

using namespace std;

class Player : public Creature
{
public:

	Player();

	void inputData(const vector<CharacterData>& allCharacters, char _code);

	CharacterData& getCharacterData();

	void discardCard(CharacterActCard card);

	void deleteIndex(int num);

	bool findIndexCards(int num);

	bool findIndexDiscard(int num);

	void addBack();

	static bool compareCharacterCard(CharacterActCard a, CharacterActCard b);

	void reuse();

	vector<CharacterActCard>& getCard();

	void setSleep(bool _sleep);

	bool getSleep();

	void setHP(int hp);

	vector<CharacterActCard>& getAllCards();

	bool choose = false;

	void setSequence(int seq);

	int getSequence(void);

	vector<CharacterActCard>& getDiscardCards();

	void setHeal(int h);

	void initialize();

	void setAttack(int a);

	void setRange(int r);

	int getRange();

	int getAttack();

	void attacked(int a);


private:

	bool loadCharacterData(const vector<CharacterData>& allCharacters);

	CharacterData data;

	vector<CharacterActCard> cards;

	vector<CharacterActCard> discardCards;


	bool sleep = false;

	int range = 0;

	int attack = 0;

	int largestHP;

};