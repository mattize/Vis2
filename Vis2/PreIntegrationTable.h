#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <array>
#include <string>
#include <iostream>

#define _USE_MATH_DEFINES 
#include <cmath>         

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "spline/src/spline.h"
#include "VulkanDevice.h"

class PreIntegrationTable {
public:
    PreIntegrationTable() {};
   
    /**
     * @brief Defines UI elements.
     *
     * This function defines the ImGui Elements and has usually to be called each iteration.
     *
     */
	void defineUI();

    /**
     * @brief Initializes UI elements.
     *
     * This function initializes the ImGui elements.
     */
    void init();

    /**
     * @brief Cleans up UI elements.
     *
     * This function cleans up all related ImGui aspects.
     */
    void cleanup();

    void setVulkanDevice(VulkanDevice vulkDevice) { 
        vulkanDevice = vulkDevice;
        device = vulkDevice.getVkDevice();
    }

    VkImageView getVolumeTableView() { return tableViews[0]; }
    VkImageView getMediumTableView() { return tableViews[1]; }
    VkImageView getRefractionTableView() { return tableViews[2]; }
    VkSampler getTableSampler() { return tableSampler; }

private:
	const int CONTROL_POINTS_COUNT = 4;
    std::array<std::string, 4> channels = { "Red", "Green", "Blue", "Alpha" };
    std::array<std::string, 3> transfer_types = { "Volume", "Medium", "Refraction" };    

    static float volume_positions[4][4];
    static int volume_values[4][4];

    static float medium_positions[4][4];
    static int medium_values[4][4];

    static float refraction_positions[4];
    static float refraction_values[4];

    std::array<tk::spline, 4> volume_curves;
    std::array<tk::spline, 4> medium_curves;
    std::array<tk::spline, 4> refraction_curves;

    VulkanDevice vulkanDevice;
    VkDevice device;

    std::array<VkImage, 3> preIntegrationTables;
    std::array<VkImageView, 3> tableViews;
    std::array<VkDeviceMemory, 3> tableMemories;
    VkSampler tableSampler;

    void applyTransportFunctions();
    uint32_t vec4ToRGBA8(const glm::vec4& color);
    void setPixel(uint32_t* pixels, glm::vec4 color, int x, int y);
    void sortDataPoints(std::vector<double>& positions, std::vector<double>& values);
    void resetTransportFunctions();
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void writeSplinesToTexture(VkImage image, std::array<tk::spline, 4> splines);
};