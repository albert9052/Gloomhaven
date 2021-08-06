#pragma once
#include <string>

#include "Position.h"

using namespace std;

enum class Type {none, player, monster};

class Creature
{
public:

	Creature() : Creature("Creature", Position(0, 0), Type::none) {};

	Creature(string _name, Position _position, Type _type);

	string getName();

	Position getPosition();

	char getCode();

	Type getType();

	void setName(string _name);

	void setPosition(Position _position);

	void setCode(char _code);

	void setType(Type _type);

	void setAgility(int a);

	int getAgility();

	virtual void setSequence(int seq);

	virtual int getSequence(void);

	virtual void initialize();

	int getHP();

	void setShield(int s);

	int getShield();

	int conflict;

	void setSecAgility(int a);

	int getSecAgility();

	bool dead = false;

protected:

	Type type;

	Position position;

	string name;

	char code;

	int creatureAgility;

	int sequence;

	int HP;

	int shield = 0;

	int secAgility;


};