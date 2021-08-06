#include "Monster.h"
#include <algorithm>

Monster::Monster(string _name, Position _position, int _mode) : Creature(_name, _position, Type::monster)
{
	mode = _mode;
	code = _name[0];
	conflict = 1;
}

void Monster::loadData(vector<MonsterData> _data)
{
	bool found = false;
	for (int i = 0; i < _data.size(); i++)
	{
		if (_data[i].name == name)
		{
			found = true;
			data = _data[i];
			cards = data.actions;
			if (mode == 1)
			{
				largestHP = data.normalLargestHP;
				HP = largestHP;
				attack = data.normalBaseATK;
				range = data.normalBaseRange;
			}
			else if (mode == 2)
			{
				largestHP = data.eliteLargestHP;
				HP = largestHP;
				attack = data.eliteBaseATK;
				range = data.eliteBaseRange;
			}
			break;
		}
	}
	if (found == false)
	{
		cout << "Error: " << endl;
		cout << "Can't find monsterData with \"" << name << "\".  (Monster.setData)" << endl;
		exit (1);
	}
}

int Monster::getMode()
{
	return mode;
}

MonsterData& Monster::getMonsterData()
{
	data.actions = cards;
	return data;
}

////////////////////////
void Monster::setMode(int _mode)
{
	mode = _mode;
	if (mode == 1)
	{
		attack = data.normalBaseATK;
		HP = data.normalLargestHP;
		largestHP = HP;
		range = data.normalBaseRange;
	}
	if (mode == 2)
	{
		attack = data.eliteBaseATK;
		HP = data.eliteLargestHP;
		largestHP = HP;
		range = data.eliteBaseRange;
	}
}

vector<MonsterActCard>& Monster::getCards()
{
	return cards;
}

vector<MonsterActCard>& Monster::getDiscardCards()
{
	return discardCards;
}

void Monster::setSequence(int seq)
{
	this->sequence = seq;
}

int Monster::getSequence(void)
{
	return this->sequence;
}

void Monster::setAttack(int att)
{
	if (mode == 1)
	{
		attack = data.normalBaseATK;
	}
	if (mode == 2)
	{
		attack = data.eliteBaseATK;
	}
	attack += att;
}

int Monster::getAttack()
{
	return attack;
}

void Monster::setHeal(int h)
{
	HP += h;
	if (HP > largestHP)
	{
		HP = largestHP;
	}
}

void Monster::initialize()
{
	if (mode == 1)
	{
		attack = data.normalBaseATK;
		range = data.normalBaseRange;
		shield = 0;
	}
	else if (mode == 2)
	{
		attack = data.eliteBaseATK;
		range = data.eliteBaseRange;
		shield = 0;
	}
	else
	{
		cout << "Error";
	}
}

void Monster::reuse()
{
	vector<MonsterActCard> temp;
	cards.insert(cards.end(), discardCards.begin(), discardCards.end());
	sort(cards.begin(), cards.end(), compare);
	discardCards = temp;
}

void Monster::attacked(int a)
{
	if (shield == 0)
	{
		HP -= a;
	}
	else
	{
		if (shield - a >= 0)
		{
			;
		}
		else
		{
			HP += shield;
			HP -= a;
		}
	}
}

void Monster::setRange(int a)
{
	if (mode == 1)
	{
		range = data.normalBaseRange;
	}
	if (mode == 2)
	{
		range = data.eliteBaseRange;
	}
	range += a;
	if (range <= 0)
	{
		range = 1;
	}
}

int Monster::getRange()
{
	return range;
}

bool Monster::compare(MonsterActCard& lhs, MonsterActCard& rhs)
{
	return lhs.number < rhs.number;
}