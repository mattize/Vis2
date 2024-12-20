#pragma once

#include "Common.h"

class VulkanDevice {
public:
    VulkanDevice() {};
    VulkanDevice(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
        : m_device(device), m_physicalDevice(physicalDevice), m_commandPool(commandPool), m_graphicsQueue(graphicsQueue) {};

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, int layerCount);

    VkDevice& getVkDevice();
    VkPhysicalDevice& getVkPhysicalDevice();
    VkDescriptorSetLayout& getVkDescriptorSetLayout();

    void setDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout);

private:
    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
    VkCommandPool m_commandPool;
    VkQueue m_graphicsQueue;
    VkDescriptorSetLayout m_descriptorSetLayout;
};

