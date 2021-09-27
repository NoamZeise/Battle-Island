#include "unit.h"

Unit::Unit(glm::vec4 worldRect, Location location, unsigned int unitTex, int health, Random random)
{
	this->random = random;
	this->worldRect = worldRect;
	this->coords = location;
	this->tileRect = helper::tileBounds(worldRect, coords);
	this->soldierTex = unitTex;
	this->usedMove = usedMove;
	this->availableMove = availableMove;
	soldiers.resize(health);
	for (size_t i = 0; i < health; i++)
	{
		soldiers[i] = glm::vec2(
			tileRect.x +
			(random.PositiveReal() * (tileRect.z - SOLDIER_TEX_DIM.x)),
			tileRect.y +
			(random.PositiveReal() * (tileRect.w - SOLDIER_TEX_DIM.y)));
	}
}



void Unit::Update(Timer& timer)
{
	if (moving)
	{
		bool allArrived = true;
		for (size_t i = 0; i < soldiers.size(); i++)
		{
			float dist = glm::distance(destination[i], soldiers[i]);
			if (dist < 0.05)
				moving = false;
			else 
				allArrived = false;
			glm::vec2 unitV((destination[i].x /* + (random.Real() * (dist / 2)) */) - soldiers[i].x, (destination[i].y /* + (random.Real() * (dist / 2)) */) - soldiers[i].y);
			unitV /= sqrt((unitV.x * unitV.x) + (unitV.y * unitV.y));
			soldiers[i] += (unitV * (SOLIDER_SPEED * timer.FrameElapsed()));
		}
	}
	else
	{
		for (size_t i = 0; i < soldiers.size(); i++)
		{
			if (random.PositiveReal() > 0.99)
			{
				soldiers[i] += glm::vec2(random.Real() * 1, random.Real() * 1);
				if (soldiers[i].x < tileRect.x) soldiers[i].x = tileRect.x;
				if (soldiers[i].y < tileRect.y) soldiers[i].y = tileRect.y;
				if (soldiers[i].x > tileRect.x + (tileRect.z - SOLDIER_TEX_DIM.x)) soldiers[i].x = tileRect.x + (tileRect.z - SOLDIER_TEX_DIM.x);
				if (soldiers[i].y > tileRect.y + (tileRect.w - SOLDIER_TEX_DIM.y)) soldiers[i].y = tileRect.y + (tileRect.w - SOLDIER_TEX_DIM.y);
			}
		}
	}
}


void Unit::Draw(Render& render)
{
	for (const auto& soldier : soldiers)
	{
		render.DrawSquare(glm::vec4(soldier.x, soldier.y, SOLDIER_TEX_DIM.x, SOLDIER_TEX_DIM.y), 0, soldierTex);
	}
}


void Unit::move(Location location)
{
	if (movesLeft == 0 || movesLeft > 2)
	{
		return;
	}
	movesLeft--;
	moving = true;
	coords = location;
	tileRect = helper::tileBounds(worldRect, coords);
	destination.resize(soldiers.size());
	for (size_t i = 0; i < destination.size(); i++)
	{
		destination[i] = glm::vec2(tileRect.x + (random.PositiveReal() * (TILE_WIDTH - SOLDIER_TEX_DIM.x)),
			tileRect.y + (random.PositiveReal() * (TILE_HEIGHT - SOLDIER_TEX_DIM.y)));
		if (destination[i].x < tileRect.x) destination[i].x = tileRect.x;
		if (destination[i].y < tileRect.y) destination[i].y = tileRect.y;
		if (destination[i].x > tileRect.x + (tileRect.z - SOLDIER_TEX_DIM.x)) destination[i].x = tileRect.x + (tileRect.z - SOLDIER_TEX_DIM.x);
		if (destination[i].y > tileRect.y + (tileRect.w - SOLDIER_TEX_DIM.y)) destination[i].y = tileRect.y + (tileRect.w - SOLDIER_TEX_DIM.y);
	}
}

bool Unit::hit()
{
	soldiers.pop_back();
	return (soldiers.size() == 0);
	
}

void Unit::addHealth(int hpAdd)
{
	for (size_t i = 0; i < hpAdd; i++)
	{
		soldiers.push_back(glm::vec2(
			tileRect.x +
			(random.PositiveReal() * (tileRect.z - SOLDIER_TEX_DIM.x)),
			tileRect.y +
			(random.PositiveReal() * (tileRect.w - SOLDIER_TEX_DIM.y))));
	}
}