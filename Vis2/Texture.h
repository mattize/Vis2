#pragma once

#include "VulkanDevice.h"

#include <stb_image.h>

class Texture {
public:

	Texture() {};
	Texture(VulkanDevice vulkanDevice) : m_vulkanDevice(vulkanDevice) { m_device = vulkanDevice.getVkDevice(); };

	/**
	 * @brief Loads a 3D volume texture.
	 *
	 * This function loads a 3D volume texture and creates the respective Vulkan handles needed for binding the texture.
	 *
	 * @param path Path to the texture.
	 * @param startIndex First index of the textures.
	 * @param endIndex Last index of the textures.
	 * @param fileType Type of texture images.
	 */
	void load3DTexture(std::string path, int startIndex, int endIndex, std::string fileType);

	/**
	 * @brief Cleans up all Vulkan object handles.
	 *
	 * This function deletes all Vulkan handles related to the texture.
	 *
	 */
	void cleanup();

	std::string getType() const { return m_type; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	int getNrChannels() const { return m_nrChannels; }
	VkFormat getFormat() const { return m_format; }

	VkImage getTexture() const { return m_textureImage; }
	VkSampler getSampler() const { return m_textureSampler; }
	VkImageView getTextureView() const { return m_textureImageView; }
	VkImageLayout getTextureLayout() const { return m_textureLayout; }

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
	VkImageLayout m_textureLayout;

	int m_nrImages;

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
};