#pragma once
#include <iostream>
#include <string>

#include "Creature.h"
#include "Position.h"
#include "MonsterData.h"

using namespace std;

class Monster : public Creature
{
public:

	Monster() : Monster("monster", Position(0, 0), 0) {};

	Monster(string _name, Position _position, int _mode);

	void loadData(vector<MonsterData> _data);

	int getMode();

	MonsterData& getMonsterData();

	void setMode(int _mode);

	vector<MonsterActCard>& getCards();

	vector<MonsterActCard>& getDiscardCards();

	void setSequence(int seq);

	int getSequence(void);

	void setAttack(int att);

	int getAttack();

	void setHeal(int h);

	int select;

	void initialize();

	void reuse();

	bool open;

	void attacked(int a);

	void setRange(int a);

	int getRange();

	static bool compare(MonsterActCard& lhs, MonsterActCard& rhs);


private:

	int mode;

	MonsterData data;

	vector<MonsterActCard> cards;

	vector<MonsterActCard> discardCards;

	int range;

	int attack;

	int largestHP;

};