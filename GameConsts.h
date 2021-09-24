#pragma once
#ifndef GAME_CONSTS_H
#define GAME_CONSTS_H

static const int TILE_WIDTH = 16;
static const int TILE_HEIGHT = 16;

struct Location
{
	Location() { x = 0, y = 0; }
	Location(unsigned int x, unsigned int y) { this->x = x; this->y = y; }
	unsigned int x = 0;
	unsigned int y = 0;

	bool operator ==(const Location* other)
	{
		return x == other->x && y == other->y;
	}
	bool operator ==(const Location other)
	{
		return x == other.x && y == other.y;
	}
};

enum Tile
{
	Empty,
	Water,
	Land,
	PlayerCity,
	EnemyCity,
	Material,
	PlayerUnit,
	EnemyUnit,
};

enum MenuElems
{
	None,
	Exit,
	Act,
	Build,
	Hit,
	Move,
	Take,
	Pass,
	Save,
	Load,
};


#endif