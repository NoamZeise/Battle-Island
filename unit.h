#pragma once
#ifndef UNIT_H
#define UNIT_H

#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>

#include <vector>
#include <utility>

#include "GameConsts.h"
#include "Render.h"
#include "MyRandom.h"
#include "GameHelper.h"
#include "Timer.h"


class Unit
{
public:
	Unit(glm::vec4 worldRect, Location location, unsigned int unitTex, int health, Random random);

	void Update(Timer& timer);

	void Draw(Render& render);

	bool isMoving() { return moving; }
	int health() { return soldiers.size(); }
	Location getLocation(){ return coords; };
	void move(Location location);
	void take() { movesLeft--; }
	unsigned int getMovesLeft() { return movesLeft; }
	void resetMoves() { movesLeft = 2; }
	bool hit();
	void addHealth(int hpAdd);
private:
	unsigned int movesLeft = 2;
	unsigned int soldierTex = 0;
	unsigned int usedMove = 0;
	unsigned int availableMove = 0;
	std::vector<glm::vec2> soldiers;
	std::vector<glm::vec2> destination;
	Location coords;
	glm::vec4 tileRect;
	bool moving = false;
	Random random;
	float SOLIDER_SPEED = 0.02f;
	glm::vec4 worldRect;
	glm::vec2 SOLDIER_TEX_DIM = glm::vec2( 3.0f, 3.0f);
};






#endif;