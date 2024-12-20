#include "Vis2.h"

void Vis2::run() {
	setWorkingDirectory();
	init();
	loadAssets();
	renderLoop();
	cleanup();
};

void Vis2::setWorkingDirectory() {
	std::filesystem::path workingDir = std::filesystem::current_path();

	while (!workingDir.empty()) {
		if (std::filesystem::exists(workingDir / "assets")) {
			std::filesystem::current_path(workingDir);
			return;
		}
		workingDir = workingDir.parent_path();
	}

	throw std::runtime_error("Project root not found. Assets folder missing.");
}

void Vis2::init() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vis2", nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
	m_vulkanHandler = VulkanHandler(m_window);
	m_vulkanHandler.initVulkan();
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	m_camera = Camera(WIDTH, HEIGHT, FOV, NEARZ, FARZ);
}

void Vis2::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<Vis2*>(glfwGetWindowUserPointer(window));
	app->m_framebufferResized = true;
}

void Vis2::renderLoop() {
	float t = float(glfwGetTime());
	float dt = 0.0f;
	float t_sum = 0.0f;

	while (!glfwWindowShouldClose(m_window)) {

		update(dt);
		draw();

		dt = t;
		t = float(glfwGetTime());
		dt = t - dt;
		t_sum += dt;
	}
}

void Vis2::loadAssets() {
	//m_vulkanHandler.createCube();
	m_vulkanHandler.createQuad();
}

void Vis2::update(float dt) {
	glfwPollEvents();

	updateCamera();
	m_vulkanHandler.update(dt, m_camera);
}

void Vis2::draw() {
	m_vulkanHandler.drawFrame();
}

void Vis2::cleanup() {
	m_vulkanHandler.cleanup();
}

void Vis2::updateCamera() {
	glfwGetCursorPos(m_window, &m_mouse_x, &m_mouse_y);
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);

	if (m_mouse_control) {
		

		if (m_firstClick) {
			glfwSetCursorPos(m_window, (width / 2), (height / 2));
			m_firstClick = false;
			m_mouse_x = width / 2;
			m_mouse_y = height / 2;
		}

		m_camera.update(width, height, (width / 2) - m_mouse_x , (height / 2) - m_mouse_y, m_deltaZoom);

		glfwSetCursorPos(m_window, (width / 2), (height / 2));
	}
	else {
		m_camera.update(width, height, 0, 0, m_deltaZoom);
		m_firstClick = true;
	}

	m_deltaZoom = 0.0f;
}

void Vis2::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	auto app = reinterpret_cast<Vis2*>(glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (!app->m_mouse_control) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			app->m_mouse_control = true;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		app->m_mouse_control = false;
	}
}

void Vis2::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto app = reinterpret_cast<Vis2*>(glfwGetWindowUserPointer(window));
	app->setDeltaZoom(float(yoffset));
};

void Vis2::setDeltaZoom(float deltaZoom) {
	m_deltaZoom = deltaZoom;
}
