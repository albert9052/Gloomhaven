#pragma once
#include "Position.h"

class Obstacle
{
public:

	Obstacle() : Obstacle(Position(0, 0), 0) {};

	Obstacle(Position _position, int _region)
	{
		position = _position;
		region = _region;
	}

	Position getPosition()
	{
		return position;
	}

	int getRegion()
	{
		return region;
	}

	void setPosition(Position _position)
	{
		position = _position;
	}

	void setRegion(int _region)
	{
		region = _region;
	}

private:

	Position position;
	int region = 0;

};