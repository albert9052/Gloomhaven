#include "Creature.h"

Creature::Creature(string _name, Position _position, Type _type)
{
	name = _name;
	position = _position;
	type = _type;
}

string Creature::getName()
{
	return name;
}

Position Creature::getPosition()
{
	return position;
}

char Creature::getCode()
{
	return code;
}

Type Creature::getType()
{
	return type;
}

void Creature::setName(string _name)
{
	name = _name;
}

void Creature::setPosition(Position _position)
{
	position = _position;
}

void Creature::setCode(char _code)
{
	code = _code;
}

void Creature::setType(Type _type)
{
	type = _type;
}

void Creature::setAgility(int a)
{
	creatureAgility = a;
}

int Creature::getAgility()
{
	return creatureAgility;
}

void Creature::setSequence(int seq)
{
	sequence = seq;
}

int Creature::getSequence()
{
	return sequence;
}

void Creature::initialize()
{
	;
}

int Creature::getHP()
{
	return HP;
}

int Creature::getShield()
{
	return shield;
}

void Creature::setShield(int s)
{
	shield += s;
}

int Creature::getSecAgility()
{
	return secAgility;
}

void Creature::setSecAgility(int sA)
{
	secAgility = sA;
}