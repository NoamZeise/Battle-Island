#pragma once
#ifndef WORLD_H
#define WORD_H

#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <map>

#include "Render.h"
#include "Timer.h"
#include "GBInput.h"


enum Tile
{
	Water,
	Land
};

class World
{
public:
	World(glm::vec2 position, unsigned int width, unsigned int height);
	
	void LoadTextures(Render* render);

	void Draw(Render* render);

	glm::vec4 getBounds();
private:
	const int TILE_WIDTH = 16;
	const int TILE_HEIGHT = 16;
	glm::vec2 position;
	unsigned int width, height;
	std::vector<std::vector<Tile>> map;
	std::map<Tile, uint32_t> texture;
};



#endif