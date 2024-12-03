#pragma once

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "VulkanHandler.h"

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

    void init();
    void setWorkingDirectory();
    void renderLoop();
    void update(float dt);
    void draw();
    void cleanup();
    void loadAssets();
    void updateCamera();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

};