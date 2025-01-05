#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <array>
#include <string>
#include <iostream>

class PreIntegrationTable {
public:
    PreIntegrationTable() {};

	void defineUI();

private:
	const int CONTROL_POINTS_COUNT = 4;
    std::array<std::string, 4> channels = { "Red", "Green", "Blue", "Alpha" };
    std::array<std::string, 3> transfer_types = { "Volume", "Medium", "Refraction" };

    
};