#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Render.h"
#include "GBInput.h"
#include "Timer.h"
#include "GameHelper.h"
#include "unit.h"
#include "GameConsts.h"

#include <iostream>
#include <vector>

class Player
{
public:
	Player(Location city, unsigned int cityTex, glm::vec4 worldRect) 
	{ 
		this->city = city;
		this->cityRect = helper::tileBounds(worldRect, city);
		this->cityTex = cityTex;
	};

	virtual void Update(Timer& timer)
	{
		for (auto& unit : units)
			unit.Update(timer);
	}

	void Draw(Render& render)
	{
		render.DrawSquare(cityRect, 0, cityTex);
		for (auto& unit : units)
			unit.Draw(render);
	}

	Location getCityLocation() { return city; }

	void moveUnit(Location currentLoc, Location newLoc)
	{
		for (int i = 0; i < units.size(); i++)
		{
			if (units[i].getLocation() == currentLoc)
			{
				units[i].move(newLoc);
				break;
			}
		}
	}

	void UnitTake(Location currentLoc)
	{
		for (int i = 0; i < units.size(); i++)
		{
			if (units[i].getLocation() == currentLoc)
			{
				units[i].take();
			}
		}
	}

	void ResetMoves()
	{
		for (int i = 0; i < units.size(); i++)
		{
			units[i].resetMoves();
		}
	}

	unsigned int UnitMoves(Location currentLoc)
	{
		for (int i = 0; i < units.size(); i++)
		{
			if (units[i].getLocation() == currentLoc)
			{
				return units[i].getMovesLeft();
			}
		}
		return 0;
	}

	void addUnit(Unit unit) { units.push_back(unit); }

	void addResource(int count)
	{
		resources++;
	}

	int getResourceCount() { return resources; }

protected:
	Location city;
	std::vector<Unit> units;
	int resources = 3;
private:
	glm::vec4 cityRect;
	unsigned int cityTex;

};

struct NPCommand
{
	Location target{ 0, 0 };
	Location destination{ 0, 0 };
	MenuElems chosenCommand = MenuElems::Pass;
};

class NPC : public Player
{
public:
	NPC(Location city, unsigned int cityTex, glm::vec4 worldRect) : Player(city, cityTex, worldRect) { }
	void Update(Timer& timer) override
	{
		Player::Update(timer);
	}

	NPCommand getNPCommand(std::vector<std::vector<Tile>>* pMap)
	{
		NPCommand command;
		return command;
	}

private:
};

class User : public Player
{
public:
	User(Location city, unsigned int cityTex, glm::vec4 worldRect) : Player(city, cityTex, worldRect) { }
	void Update(Timer& timer) override
	{
		Player::Update(timer);
	}
private:
};


#endif