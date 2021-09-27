#pragma once
#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <fstream>

#include "typeStructs.h"
#include "vkhelper.h"
#include "Consts.h"

struct TempTexture
{
	std::string path;
	unsigned char* pixelData;
	int width;
	int height;
	int nrChannels;
	VkFormat format;
	VkDeviceSize fileSize;
};

struct Texture
{
	Texture(){}
	Texture(TempTexture tex)
	{
		width = tex.width;
		height = tex.height;
		mipLevels = std::floor(std::log2(width > height ? width : height)) + 1;
	}
	uint32_t width;
	uint32_t height;
	VkImage image;
	VkImageView view;
	uint32_t mipLevels;
	VkDeviceSize imageMemSize;
};

class TextureLoader
{
public:
	TextureLoader() {};
	TextureLoader(Base base, VkCommandPool pool);
	~TextureLoader();
	uint32_t loadTexture(std::string path);
	uint32_t loadTexture(unsigned char* data, int width, int height, int nrChannels);
	VkImageView getImageView(uint32_t texID);
	void endLoading();

	VkSampler sampler;

private:
	Base base;
	VkCommandPool pool;

	std::vector<TempTexture> texToLoad;
	std::vector<Texture> textures;
	VkDeviceMemory memory;

	bool PALETTE_SWAP = false;
	//lighter to darker
	unsigned char COLOUR_SWAP_1[3] = { 0xE0, 0xF8, 0xD0 };
	unsigned char COLOUR_SWAP_2[3] = { 0x88, 0xC0, 0x70 };
	unsigned char COLOUR_SWAP_3[3] = { 0x34, 0x68, 0x56 };
	unsigned char COLOUR_SWAP_4[3] = { 0x08, 0x18, 0x20 };
};






#endif