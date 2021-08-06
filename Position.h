#pragma once

class Position
{
public:

	Position() : Position(0, 0) {};

	Position(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	void operator=(Position _pos)
	{
		x = _pos.x;
		y = _pos.y;
	}

	bool operator==(Position _pos)
	{
		if (x == _pos.x && y == _pos.y)
			return true;
		return false;
	}

	bool operator!=(Position _pos)
	{
		if (x != _pos.x || y != _pos.y)
			return true;
		return false;
	}

	int x, y;
};