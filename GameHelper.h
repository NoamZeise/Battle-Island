#pragma once
#ifndef GAME_HELPER_H
#define GAME_HELPER_H

#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>

#include "GameConsts.h"

static class helper
{
public:
	static bool contains(glm::vec2 point, glm::vec4 rect)
	{
		return !(point.x < rect.x || point.y < rect.y || point.x > rect.x + rect.z || point.y > rect.y + rect.w);
	}
	static bool colliding(glm::vec4 rect1, glm::vec4 rect2)
	{
		return (rect1.x < rect2.x + rect2.z &&
			rect1.x + rect1.z > rect2.x &&
			rect1.y < rect2.y + rect2.w &&
			rect1.y + rect1.w > rect2.y);
	}
	static glm::vec4 tileBounds(glm::vec4 worldRect, Location location)
	{
		return glm::vec4(worldRect.x + (location.x * TILE_WIDTH),
			worldRect.y + (location.y * TILE_HEIGHT),
			TILE_WIDTH, TILE_HEIGHT);
	}
};


#endif