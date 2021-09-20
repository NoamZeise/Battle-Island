#include "World.h"


World::World(glm::vec2 position, unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	map.push_back(std::vector<Tile>(width * height, Tile::Water));
}

void World::LoadTextures(Render* render)
{
	texture.insert(std::pair<Tile, uint32_t>(Tile::Water, render->LoadTexture("textures/tiles/water.png")));
}

void World::Draw(Render* render)
{
	for (const auto& layer : map)
	{
		int i = 0;
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				render->DrawSquare(glm::vec4(
					position.x + (x * TILE_WIDTH), position.y + (y * TILE_HEIGHT),
					TILE_WIDTH, TILE_HEIGHT), 0, texture[layer[i]]);
				i++;
			}
		}
	}
}

glm::vec4 World::getBounds()
{
	return glm::vec4(position.x, position.y, width * TILE_WIDTH, height * TILE_HEIGHT);
}