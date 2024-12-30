#pragma once

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "VulkanHandler.h"

#include <fstream>
#include <iterator>
#include <vector>

class Vis2 {
public:
	void run();
    void setDeltaZoom(float deltaZoom);

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    const float ASPECT = float(WIDTH) / float(HEIGHT);
    const float FOV = 60.0f;
    const float NEARZ = 0.1f;
    const float FARZ = 300.0f;
    
    glm::vec2 planeSides = glm::vec2(0.6, 0.6);
    int numPlanes = 400;
    float voxelDepth = 1.0;

    Texture m_volume;
    uint32_t volume_width;
    uint32_t volume_height;

    Light m_light;

    GLFWwindow* m_window;
    bool m_framebufferResized = false;
    bool m_mouse_control = false;
    bool m_firstClick = false;
    double m_mouse_x;
    double m_mouse_y;
    int m_lastMouse_x = 0;
    int m_lastMouse_y = 0;

    float m_deltaZoom;

    Camera m_camera;
    VulkanHandler m_vulkanHandler;
    VulkanDevice m_vulkanDevice;

    void init();
    void setWorkingDirectory();
    void renderLoop();
    void cleanup();
    void loadAssets();
    void updateCamera();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

};