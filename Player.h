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
		if(cityHealth > 0)
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

	int unitCount() { return units.size(); }

	void UnitTurn(Location loc)
	{
		if (loc == city)
		{
			cityMoves--;
		}
		else
		{
			for (int i = 0; i < units.size(); i++)
			{
				if (units[i].getLocation() == loc)
				{
					units[i].take();
				}
			}
		}
	}

	void ResetMoves()
	{
		cityMoves = 2;
		for (int i = 0; i < units.size(); i++)
		{
			units[i].resetMoves();
		}
	}

	

	unsigned int movesLeft(Location loc)
	{
		if (loc == city)
		{
			return cityMoves;
		}
		else
		{
			for (int i = 0; i < units.size(); i++)
			{
				if (units[i].getLocation() == loc)
				{
					return units[i].getMovesLeft();
				}
			}
		}
		return 0;
	}

	void addUnit(Unit unit) { units.push_back(unit); }
	void buildUnit(Unit unit) 
	{ 
		resources -= 3;
		cityMoves--;
		units.push_back(unit);
	}

	void addResource(int count)
	{
		resources+=count;
	}

	int getResourceCount() { return resources; }

	bool hitUnitOrCity(Location loc)
	{
		if (loc == city)
		{
			cityHealth--;
			return cityHealth <= 0;
		}
		else
			for (int i = 0; i < units.size(); i++)
			{
				if (units[i].getLocation() == loc)
				{
					if (units[i].hit())
					{
						units.erase(units.begin() + i);

						return true;
					}
				}
				return false;
			}
	}
	int getCityHealth() { return cityHealth; }

protected:
	Location city;
	std::vector<Unit> units;
	int resources = 0;
	glm::vec4 cityRect;
	int cityHealth = 5;
	int cityMoves = 2;
private:
	unsigned int cityTex;

};

struct NPCommand
{
	std::vector<Location> unmovedUnits;
	Location city;
	bool cityMove = false;
};

class NPC : public Player
{
public:
	NPC(Location city, unsigned int cityTex, glm::vec4 worldRect) : Player(city, cityTex, worldRect) { }
	void Update(Timer& timer) override
	{
		Player::Update(timer);
	}

	NPCommand getNPCommand()
	{
		NPCommand command; //defaults to turn finish command

		for (size_t i = 0; i < units.size(); i++)
		{
			if (units[i].getMovesLeft() > 0)
				command.unmovedUnits.push_back(units[i].getLocation());
		}
		if (cityMoves > 0)
		{
			command.cityMove = true;
			command.city = city;
		}

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