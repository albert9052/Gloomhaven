#include "Player.h"

Player::Player() : Creature()
{
	type = Type::player;
	conflict = 0;
}

void Player::inputData(const vector<CharacterData> &allCharacters, char _code)
{
	while (1)
	{
		cin >> name;
		code = _code;
		bool found = loadCharacterData(allCharacters);
		if (found == false)
		{
			string tempStr;
			getline(cin, tempStr);
			cout << "Input error: " << endl;
			cout << "找尋不到名叫 " << name << " 的角色，請重新輸入(角色名稱 手牌(根據角色決定張數))：" << endl;
			continue;
		}
		HP = data.HP;
		largestHP = data.HP;
		string tempStr;
		cin.ignore();
		getline(cin, tempStr);
		stringstream ss(tempStr);
		int tempNum;
		bool getCard;
		bool* chosenCards = new bool[data.actions.size()];
		memset(chosenCards, false, sizeof(bool) * data.actions.size());
		while (ss >> tempNum)
		{
			getCard = false;
			for (int i = 0; i < data.actions.size(); i++)
			{
				if (tempNum == data.actions[i].number && chosenCards[i] == false)
				{
					cards.push_back(data.actions[i]);
					chosenCards[i] = true;
					getCard = true;
				}
			}
			if (getCard == false)
			{
				cout << "Input error: " << endl;
				cout << "其中有不存在或重複選擇的卡牌，請重新輸入(角色名稱 手牌(根據角色決定張數))：" << endl;
				cards.clear();
				break;
			}
		}
		if (getCard == false)
		{
			continue;
		}

		if (cards.size() != data.initCards)
		{
			cards.clear();
			cout << "Input error: " << endl;
			cout << "手牌數量不符合，請重新輸入(角色名稱 手牌(" << data.initCards << "張))：" << endl;
			continue;
		}
		break;
	}
}

bool Player::loadCharacterData(const vector<CharacterData> &allCharacters)
{
	bool found = false;
	for (int i = 0; i < allCharacters.size(); i++)
	{
		//cout << "(" << characterName << ", "  << allCharacters[i].name << ")" << endl;
		if (allCharacters[i].name == name)
		{
			data = allCharacters[i];
			largestHP = data.largestHP;
			HP = largestHP;
			found = true;
			break;
		}
	}
	if (found == false)
	{
		return false;
	}
	return true;
}

CharacterData& Player::getCharacterData()
{
	data.actions = cards;
	return data;
}

void Player::discardCard(CharacterActCard card)
{
	discardCards.push_back(card);
}

void Player::deleteIndex(int num)
{
	for (int i = 0; i < cards.size(); i++)
	{
		if (cards.at(i).number == num)
		{
			discardCard(cards.at(i));
			cards.erase(cards.begin() + i);
			break;
		}
	}
}

bool Player::findIndexCards(int num)
{
	for (int i = 0; i < cards.size(); i++)
	{
		if (cards.at(i).number == num)
		{
			return true;
		}
	}
	return false;
}

bool Player::findIndexDiscard(int num)
{
	for (int i = 0; i < discardCards.size(); i++)
	{
		if (discardCards.at(i).number == num)
		{
			return true;
		}
	}
	return false;
}

void Player::addBack()
{
	cards.insert(cards.end(), discardCards.begin(), discardCards.end());
	sort(cards.begin(), cards.end(), compareCharacterCard);
}

bool Player::compareCharacterCard(CharacterActCard a, CharacterActCard b)
{
	return a.number < b.number;
}

void Player::reuse()
{
	cards.insert(cards.begin(), discardCards.begin(), discardCards.end());
	sort(cards.begin(), cards.end(), compareCharacterCard);
	vector<CharacterActCard> temp;
	discardCards = temp;
}


//////////////////////////////////
vector<CharacterActCard>& Player::getCard()
{
	vector<CharacterActCard> ret;
	if (discardCards.size() >= 2)
	{
		ret.push_back(discardCards.at(discardCards.size() - 2));
		ret.push_back(discardCards.at(discardCards.size() - 1));
	}
	return ret;
}

void Player::setSleep(bool _sleep)
{
	sleep = _sleep;
}

bool Player::getSleep()
{
	return sleep;
}

void Player::setHP(int hp)
{
	if (data.largestHP < HP + hp)
	{
		HP = data.largestHP;
	}
	else
	{
		HP += hp;
	}
}

vector<CharacterActCard>& Player::getAllCards()
{
	return cards;
}

void Player::setSequence(int seq)
{
	this->sequence = seq;
}

int Player::getSequence(void)
{
	return this->sequence;
}

vector<CharacterActCard>& Player::getDiscardCards()
{
	return discardCards;
}


void Player::setHeal(int h)
{
	HP += h;
	if (HP > largestHP)
	{
		HP = largestHP;
	}
}

void Player::initialize()
{
	shield = 0;
	attack = 0;
}

void Player::setAttack(int a)
{
	attack = a;
}

void Player::setRange(int r)
{
	range = r;
}

int Player::getRange()
{
	return range;
}

int Player::getAttack()
{
	return attack;
}

void Player::attacked(int a)
{
	if (a <= shield)
	{
		;
	}
	else
	{
		HP += shield;
		HP -= a;
	}
}