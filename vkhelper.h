#pragma once
#ifndef VKHELPER_H
#define VKHELPER_H

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

#include "typeStructs.h"

struct vkhelper
{
	static uint32_t findMemoryIndex(VkPhysicalDevice physicalDevice, uint32_t memoryTypeBits, VkMemoryPropertyFlags properties);
	static glm::mat4 getModel(glm::vec4 drawRect, float rotate);
	static glm::vec4 getTextureOffset(glm::vec4 drawArea, glm::vec4 textureArea);
	static void createBufferAndMemory(Base base, VkDeviceSize size, VkBuffer* buffer, VkDeviceMemory* memory,
		VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	static void createMemory(Base base, VkDeviceSize size, VkDeviceMemory* memory,
		VkMemoryPropertyFlags properties, uint32_t memoryTypeBits);

	//from https://stackoverflow.com/questions/17261798/converting-a-hex-string-to-a-byte-array
	static int char2int(unsigned char input)
	{
		if (input >= '0' && input <= '9')
			return input - '0';
		if (input >= 'A' && input <= 'F')
			return input - 'A' + 10;
		if (input >= 'a' && input <= 'f')
			return input - 'a' + 10;
		return 0x00;
	}
	static void hex2bin(const char* src, unsigned char* target)
	{
		while (*src && src[1])
		{
			*(target++) = char2int(*src) * 16 + char2int(src[1]);
			src += 2;
		}
	}

};




#endif