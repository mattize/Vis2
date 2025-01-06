/*#pragma once

#include "Common.h"
#include "Camera.h"
#include "VulkanDevice.h"

class EnvironmentMap {
public:
	EnvironmentMap() {};
	EnvironmentMap(VulkanDevice vulkanDevice);

	VkBuffer& getVertexBuffer();
	VkBuffer& getIndexBuffer();
	VkDescriptorSet& getDescriptorSet(int frame);
	std::vector<uint32_t> getIndices();
	VkPipeline getPipeline();

	void update(uint32_t currentImage, Camera& camera);
	void cleanup();

private:
	VulkanDevice m_vulkanDevice;
	VkDevice m_device;

	glm::vec3 m_position;
	glm::vec3 m_vdirection;
	glm::mat4 m_model_mat = glm::mat4(1.0f);

	std::vector<VkBuffer> m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<void*> m_uniformBuffersMapped;

	VkDescriptorPool m_descriptorPool;
	std::vector<VkDescriptorSet> m_descriptorSets;

	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
};*/