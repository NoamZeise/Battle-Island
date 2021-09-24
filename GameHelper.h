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
	static glm::vec4 tileBounds(glm::vec4 worldRect, Location location)
	{
		return glm::vec4(worldRect.x + (location.x * TILE_WIDTH),
			worldRect.y + (location.y * TILE_HEIGHT),
			TILE_WIDTH, TILE_HEIGHT);
	}
};


#endif