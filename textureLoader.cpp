#include "TextureLoader.h"

TextureLoader::TextureLoader(Base base, VkCommandPool pool)
{
	this->base = base;
	this->pool = pool;
	std::ifstream paletteFile("paletteSwap.txt");
	if (paletteFile.good())
	{
		PALETTE_SWAP = true;
		if (paletteFile.is_open()) {
			std::string line;
			int i = 0;
			while (std::getline(paletteFile, line)) {
				if (line.size() >= 6)
				{
					unsigned char palette[3] = { 0x00, 0x00, 0x00 };

					char sHex1[2] = { line[0], line[1] };
					unsigned char bHex1 = 0x00;
					vkhelper::hex2bin(sHex1, &bHex1);
					palette[0] = bHex1;

					char sHex2[2] = { line[2], line[3] };
					unsigned char bHex2 = 0x00;
					vkhelper::hex2bin(sHex2, &bHex2);
					palette[1] = bHex2;

					char sHex3[2] = { line[4], line[5] };
					unsigned char bHex3 = 0x00;
					vkhelper::hex2bin(sHex3, &bHex3);
					palette[2] = bHex3;

					switch (i)
					{
					case 0:
						COLOUR_SWAP_1[0] = palette[0];
						COLOUR_SWAP_1[1] = palette[1];
						COLOUR_SWAP_1[2] = palette[2];
							break;
					case 1:
						COLOUR_SWAP_2[0] = palette[0];
						COLOUR_SWAP_2[1] = palette[1];
						COLOUR_SWAP_2[2] = palette[2];
						break;
					case 2:
						COLOUR_SWAP_3[0] = palette[0];
						COLOUR_SWAP_3[1] = palette[1];
						COLOUR_SWAP_3[2] = palette[2];
						break;
					case 3:
						COLOUR_SWAP_4[0] = palette[0];
						COLOUR_SWAP_4[1] = palette[1];
						COLOUR_SWAP_4[2] = palette[2];
						break;
					}
				}
				i++;
			}
			paletteFile.close();
		}
	}
}

TextureLoader::~TextureLoader()
{
	if (textures.size() <= 0)
		return;
	for (const auto& tex : textures)
	{
		vkDestroyImageView(base.device, tex.view, nullptr);
		vkDestroyImage(base.device, tex.image, nullptr);
	}
	vkDestroySampler(base.device, sampler, nullptr);
	vkFreeMemory(base.device, memory, nullptr);
}

uint32_t TextureLoader::loadTexture(std::string path)
{
	texToLoad.push_back({ path });
	TempTexture* tex = &texToLoad.back();
	tex->pixelData = stbi_load(tex->path.c_str(), &tex->width, &tex->height, &tex->nrChannels, 0);
	if (!tex->pixelData)
		throw std::runtime_error("failed to load texture at " + path);
	tex->fileSize = tex->width * tex->height * tex->nrChannels;

	switch (tex->nrChannels)
	{
	case 1:
		if(USE_SRGB)
			tex->format = VK_FORMAT_R8_SRGB;
		else
			tex->format = VK_FORMAT_R8_UNORM;
		break;
	case 2:
		if (USE_SRGB)
			tex->format = VK_FORMAT_R8G8_SRGB;
		else
			tex->format = VK_FORMAT_R8G8_UNORM;
		break;
	case 3:
		if (USE_SRGB)
			tex->format = VK_FORMAT_R8G8B8_SRGB;
		else
			tex->format = VK_FORMAT_R8G8B8_UNORM;
		break;
	case 4:
		if (USE_SRGB)
			tex->format = VK_FORMAT_R8G8B8A8_SRGB;
		else
			tex->format = VK_FORMAT_R8G8B8A8_UNORM;
		break;
	default:
		throw std::runtime_error("texture at " + path + " has an unsupported number of channels");
	}

	if (PALETTE_SWAP && tex->nrChannels == 4)
	{
		for (size_t i = 0; i < tex->fileSize; i += 4)
		{
			if (tex->pixelData[i + 3] == 0x00) //transparent 
				continue;
			unsigned char colour[3];
			colour[0] = tex->pixelData[i];
			colour[1] = tex->pixelData[i + 1];
			colour[2] = tex->pixelData[i + 2];
			if (colour[0] == 0xe0 && colour[1] == 0xf8 && colour[2] == 0xd0)
			{
				tex->pixelData[i] = COLOUR_SWAP_1[0];
				tex->pixelData[i + 1] = COLOUR_SWAP_1[1];
				tex->pixelData[i + 2] = COLOUR_SWAP_1[2];
			}
			if (colour[0] == 0x88 && colour[1] == 0xc0 && colour[2] == 0x70)
			{
				tex->pixelData[i] = COLOUR_SWAP_2[0];
				tex->pixelData[i + 1] = COLOUR_SWAP_2[1];
				tex->pixelData[i + 2] = COLOUR_SWAP_2[2];
			}
			if (colour[0] == 0x34 && colour[1] == 0x68 && colour[2] == 0x56)
			{
				tex->pixelData[i] = COLOUR_SWAP_3[0];
				tex->pixelData[i + 1] = COLOUR_SWAP_3[1];
				tex->pixelData[i + 2] = COLOUR_SWAP_3[2];
			}
			if (colour[0] == 0x08 && colour[1] == 0x18 && colour[2] == 0x20)
			{
				tex->pixelData[i] = COLOUR_SWAP_4[0];
				tex->pixelData[i + 1] = COLOUR_SWAP_4[1];
				tex->pixelData[i + 2] = COLOUR_SWAP_4[2];
			}
		}
	}

	return texToLoad.size() - 1;
}


uint32_t TextureLoader::loadTexture(unsigned char* data, int width, int height, int nrChannels)
{
	texToLoad.push_back({ "NULL" });
	TempTexture* tex = &texToLoad.back();
	tex->pixelData = data;
	tex->width = width;
	tex->height = height;
	tex->nrChannels = nrChannels;
	tex->fileSize = tex->width * tex->height * tex->nrChannels;

	switch (tex->nrChannels)
	{
	case 1:
		tex->format = VK_FORMAT_R8_SRGB;
		break;
	case 2:
		tex->format = VK_FORMAT_R8G8_SRGB;
		break;
	case 3:
		tex->format = VK_FORMAT_R8G8B8_SRGB;
		break;
	case 4:
		tex->format = VK_FORMAT_R8G8B8A8_SRGB;
		break;
	default:
		throw std::runtime_error("texture character has an unsupported number of channels");
	}

	if (PALETTE_SWAP && tex->nrChannels == 4)
	{
		for (size_t i = 0; i < tex->fileSize; i += 4)
		{
			if (tex->pixelData[i + 3] == 0x00) //transparent 
				continue;
			unsigned char colour[3];
			colour[0] = tex->pixelData[i];
			colour[1] = tex->pixelData[i + 1];
			colour[2] = tex->pixelData[i + 2];
			if (colour[0] == 0xe0 && colour[1] == 0xf8 && colour[2] == 0xd0)
			{
				tex->pixelData[i] = COLOUR_SWAP_1[0];
				tex->pixelData[i + 1] = COLOUR_SWAP_1[1];
				tex->pixelData[i + 2] = COLOUR_SWAP_1[2];
			}
			if (colour[0] == 0x88 && colour[1] == 0xc0 && colour[2] == 0x70)
			{
				tex->pixelData[i] = COLOUR_SWAP_2[0];
				tex->pixelData[i + 1] = COLOUR_SWAP_2[1];
				tex->pixelData[i + 2] = COLOUR_SWAP_2[2];
			}
			if (colour[0] == 0x34 && colour[1] == 0x68 && colour[2] == 0x56)
			{
				tex->pixelData[i] = COLOUR_SWAP_3[0];
				tex->pixelData[i + 1] = COLOUR_SWAP_3[1];
				tex->pixelData[i + 2] = COLOUR_SWAP_3[2];
			}
			if (colour[0] == 0x08 && colour[1] == 0x18 && colour[2] == 0x20)
			{
				tex->pixelData[i] = COLOUR_SWAP_4[0];
				tex->pixelData[i + 1] = COLOUR_SWAP_4[1];
				tex->pixelData[i + 2] = COLOUR_SWAP_4[2];
			}
		}
	}

	return texToLoad.size() - 1;
}


void TextureLoader::endLoading()
{
	if (texToLoad.size() <= 0)
		return;

	if (texToLoad.size() > MAX_TEXTURES_SUPPORTED)
		throw std::runtime_error("not enough storage for textures");
	textures.resize(texToLoad.size());

	VkDeviceSize totalFilesize = 0;
	for (const auto& tex : texToLoad)
		totalFilesize += tex.fileSize;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;

	vkhelper::createBufferAndMemory(base, totalFilesize, &stagingBuffer, &stagingMemory,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vkBindBufferMemory(base.device, stagingBuffer, stagingMemory, 0);
	void* pMem;
	vkMapMemory(base.device, stagingMemory, 0, totalFilesize, 0, &pMem);

	//all during loop:

	//move image pixel data to buffer
	VkDeviceSize finalMemSize = 0;
	VkMemoryRequirements memreq;
	VkDeviceSize bufferOffset = 0;

	uint32_t memoryTypeBits = 0;
	uint32_t minMips = UINT32_MAX;
	for (size_t i = 0; i < texToLoad.size(); i++)
	{
		//copy texture from pixel data to mappable gpu memory
		memcpy(static_cast<char*>(pMem) + bufferOffset, texToLoad[i].pixelData, texToLoad[i].fileSize);

		if (texToLoad[i].path != "NULL") 
			stbi_image_free(texToLoad[i].pixelData);
		else
			delete texToLoad[i].pixelData;
		texToLoad[i].pixelData = nullptr;

		bufferOffset += texToLoad[i].fileSize;

		textures[i] = Texture(texToLoad[i]);
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(base.physicalDevice, texToLoad[i].format, &formatProperties);
		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)
			|| !(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT)
			|| !ENABLE_MIP)
			textures[i].mipLevels = 1;
		//get smallest mip levels of any texture
		if (textures[i].mipLevels < minMips)
			minMips = textures[i].mipLevels;


		//create image
		VkImageCreateInfo imageInfo{ VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = textures[i].width;
		imageInfo.extent.height = textures[i].height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = textures[i].mipLevels;
		imageInfo.arrayLayers = 1;
		imageInfo.format = texToLoad[i].format;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT; //todo add multsampling
		
		if (vkCreateImage(base.device, &imageInfo, nullptr, &textures[i].image) != VK_SUCCESS)
			throw std::runtime_error("failed to create image from texture at: " + texToLoad[i].path);

		//get memory requirements for image
		vkGetImageMemoryRequirements(base.device, textures[i].image, &memreq);
		memoryTypeBits |= memreq.memoryTypeBits;
		textures[i].imageMemSize = memreq.size;

		if (textures[i].imageMemSize %  memreq.alignment != 0)
			textures[i].imageMemSize = textures[i].imageMemSize + memreq.alignment
			- (textures[i].imageMemSize % memreq.alignment);

		finalMemSize += textures[i].imageMemSize;
	}
	//create device local memory for permanent storage of images
	vkhelper::createMemory(base, finalMemSize, &memory, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memoryTypeBits);


	//transition image to required format
//create command buffer
	VkCommandBufferAllocateInfo cmdAllocInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdAllocInfo.commandBufferCount = 1;
	cmdAllocInfo.commandPool = pool;
	VkCommandBuffer tempCmdBuffer;
	vkAllocateCommandBuffers(base.device, &cmdAllocInfo, &tempCmdBuffer);
	//begin command buffer
	VkCommandBufferBeginInfo cmdBeginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(tempCmdBuffer, &cmdBeginInfo);

	//create image memory barrier for layout transition - info true for all images
	VkImageMemoryBarrier barrier{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
	barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL; //for mipmapping
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	//region to copy
	VkBufferImageCopy region{};
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	bufferOffset = 0;
	VkDeviceSize finalMemoryOffset = 0;
	for (int i = 0; i < textures.size(); i++)
	{
		vkBindImageMemory(base.device, textures[i].image, memory, finalMemoryOffset);
		finalMemoryOffset += textures[i].imageMemSize;
		//transition layout cmd
		barrier.image = textures[i].image;
		barrier.subresourceRange.levelCount = textures[i].mipLevels;
		vkCmdPipelineBarrier(tempCmdBuffer,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr, 0, nullptr, 1, &barrier);

		region.imageExtent = { textures[i].width, textures[i].height, 1 };
		region.bufferOffset = bufferOffset;
		bufferOffset += texToLoad[i].fileSize;

		vkCmdCopyBufferToImage(tempCmdBuffer, stagingBuffer, textures[i].image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &region);
	}
	//end cmd and submit for execution
	if (vkEndCommandBuffer(tempCmdBuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to end command buffer");
	VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &tempCmdBuffer;
	vkQueueSubmit(base.queue.graphicsPresentQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(base.queue.graphicsPresentQueue);
	//free staging buffer/memory
	vkUnmapMemory(base.device, stagingMemory);
	vkDestroyBuffer(base.device, stagingBuffer, nullptr);
	vkFreeMemory(base.device, stagingMemory, nullptr);

	//begin command buffer for blitting
	vkResetCommandPool(base.device, pool, 0);
	vkBeginCommandBuffer(tempCmdBuffer, &cmdBeginInfo);
	
	//generate mipmaps
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	for (const auto& tex : textures)
	{
		//if (tex.mipLevels == 1)
		//	continue;
		barrier.image = tex.image;
		int32_t mipW = tex.width;
		int32_t mipH = tex.height;
		//for each mip level insert a pipeline barrier to blit image and change layout to shader-read-only
		for (size_t i = 1; i < tex.mipLevels; i++) //start at one as 0 is original image
		{
			//transfer previous image to be optimal for image transfer source
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(tempCmdBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr, 0, nullptr, 1, &barrier);

			//blit current image from previous one
			VkImageBlit blit{};
			//src info
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipW, mipH, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			//dst info
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipW > 1 ? mipW / 2 : 1, mipH > 1 ? mipH / 2: 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(tempCmdBuffer, tex.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				tex.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

			//change previous image layout to shader read only
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(tempCmdBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr, 0, nullptr, 1, &barrier);

			if (mipW > 1) mipW /= 2;
			if (mipH > 1) mipH /= 2;
		}
		//transition last mip level to shader read only
		barrier.subresourceRange.baseMipLevel = tex.mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(tempCmdBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr, 0, nullptr, 1, &barrier);
	}

	if (vkEndCommandBuffer(tempCmdBuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to end command buffer");
	vkQueueSubmit(base.queue.graphicsPresentQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(base.queue.graphicsPresentQueue);

	//create image views
	for (size_t i = 0; i < textures.size(); i++)
	{
		VkImageViewCreateInfo viewInfo{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		viewInfo.image = textures[i].image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = texToLoad[i].format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = textures[i].mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(base.device, &viewInfo, nullptr, &textures[i].view) != VK_SUCCESS)
			throw std::runtime_error("Failed to create image view from texture at: " + texToLoad[i].path);
	}

	//create image sampler
	VkPhysicalDeviceProperties deviceProps{};
	vkGetPhysicalDeviceProperties(base.physicalDevice, &deviceProps);
	VkSamplerCreateInfo samplerInfo{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	if (PIXELATED)
	{
		samplerInfo.magFilter = VK_FILTER_NEAREST;
		samplerInfo.minFilter = VK_FILTER_NEAREST;
	}
	else
	{
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
	}
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = deviceProps.limits.maxSamplerAnisotropy;
	//samplerInfo.maxAnisotropy = deviceProps.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.maxLod = static_cast<float>(minMips);
	samplerInfo.minLod = 0.0f;
	if (vkCreateSampler(base.device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
		throw std::runtime_error("Failed create sampler");

	vkFreeCommandBuffers(base.device, pool, 1, &tempCmdBuffer);
}

VkImageView TextureLoader::getImageView(uint32_t texID)
{
	if (texID < textures.size())
		return textures[texID].view;
	else if (textures.size() > 0)
		return textures[0].view;
	else
		throw std::runtime_error("no textures to replace error id with");
}