#pragma once

#include "Common.h"
#include "VulkanDevice.h"
#include <stb_image.h>

class Texture {
public:

	Texture(VulkanDevice vulkanDevice) : m_vulkanDevice(vulkanDevice) { m_device = vulkanDevice.getVkDevice(); };

	void load3DTexture(std::string path, int startIndex, int endIndex, std::string fileType);

	std::string getType() const { return m_type; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	int getNrChannels() const { return m_nrChannels; }
	VkFormat getFormat() const { return m_format; }

private:
	VulkanDevice m_vulkanDevice;
	VkDevice m_device;
	VkDeviceSize m_imageSize;

	std::string m_type;
	int m_width, m_height, m_nrChannels;
	VkFormat m_format;
	
	VkImage m_textureImage;
	VkDeviceMemory m_textureImageMemory;
	VkImageView m_textureImageView;
	VkSampler m_textureSampler;

	int m_nrImages;

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void createTextureImage();
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
};