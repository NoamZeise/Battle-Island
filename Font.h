#pragma once
#ifndef FONT_H
#define FONT_H

#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

#include "TextureLoader.h"

struct Character
{
	Character(unsigned int TextureID, glm::vec2 Size, glm::vec2 Bearing, double Advance)
	{
		this->TextureID = TextureID;
		this->Size = Size;
		this->Bearing = Bearing;
		this->Advance = Advance;
	}
	~Character()
	{

	}
	unsigned int TextureID;
	glm::vec2 Size;
	glm::vec2 Bearing;
	double Advance;
};

class Font
{
public:
	Font(std::string file, TextureLoader* texLoader);
	~Font();
	Character* getChar(char c);
private:
	std::map<char, Character*> _chars;
	bool loadCharacter(TextureLoader* textureLoader, FT_Face f, char c);
	const int SIZE = 50;
};





#endif