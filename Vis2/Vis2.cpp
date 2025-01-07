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

	m_vulkanDevice = VulkanDevice(m_vulkanHandler.getDevice(), m_vulkanHandler.getPhysicalDevice(), m_vulkanHandler.getCommandPool(),
		m_vulkanHandler.getGraphicsQueue());

	m_integrationTable.setVulkanDevice(m_vulkanDevice);
	m_integrationTable.init();
	
	m_volume = Texture(m_vulkanDevice);
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	m_camera = Camera(WIDTH, HEIGHT, FOV, NEARZ, FARZ);
	m_vulkanHandler.initUI();
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

		glfwPollEvents();

		updateCamera();
		

		const glm::mat4& cameraView = m_camera.getViewMat();
		const glm::mat4& inverseCameraView = glm::inverse(cameraView);

		float sphereRadius = 0.5;

		glm::mat3 viewMatrix3 = glm::mat3(cameraView);
		glm::vec3 originVS(cameraView * glm::vec4(0, 0, 0, 1));
		glm::vec3 middleOfObjectOnPlaneVS = originVS + glm::vec3(0.0, 0.0, std::min(-originVS.z, sphereRadius));

		glm::vec3 middleOfPlaneVS = middleOfObjectOnPlaneVS;
		glm::vec3 middleOfPlaneVSOpp = middleOfPlaneVS - glm::vec3(0, 0, sphereRadius + (middleOfPlaneVS.z - originVS.z));
		float planeDistance = glm::length(middleOfPlaneVS - middleOfPlaneVSOpp) / (float)numPlanes;

		m_vulkanHandler.updateMVP(dt, m_camera);
		m_vulkanHandler.updateAlgo(inverseCameraView, cameraView, planeDistance, middleOfPlaneVS, sphereRadius, planeSides, glm::ivec2(volume_width, volume_height),
			glm::vec3(0.25, 0.5, 0.75), glm::vec4(1.0, 1.34, 1.45, 1.8), voxelDepth, (float)volume_width, (float)volume_height, m_light);

		m_vulkanHandler.dispatchCompute(512, 512, 2);
		m_vulkanHandler.runAlgo(numPlanes, middleOfPlaneVS, planeDistance);
		m_vulkanHandler.drawFrame(numPlanes, middleOfPlaneVS, planeDistance, m_integrationTable);	

		dt = t;
		t = float(glfwGetTime());
		dt = t - dt;
		t_sum += dt;
	}
}

void Vis2::loadAssets() {
	m_light = Light(1.0f, glm::vec3(1.0, 1.0, 1.0) * 3.0f, glm::vec3(0, 0, 15));
	
	//load data and create 3D Texture here
	volume_width = 512;
	volume_height = 512;

	m_volume.load3DTexture(".\\assets\\data\\reptile\\jpgs\\", 1, 510, ".jpg");

	//m_vulkanHandler.createCube();
	m_vulkanHandler.createQuad(m_volume, m_integrationTable);
}

void Vis2::cleanup() {	
	m_vulkanHandler.setDeviceWaitIdle();

	m_integrationTable.cleanup();
	m_volume.cleanup();
	m_vulkanHandler.cleanup();
}

void Vis2::updateCamera() {
	glfwGetCursorPos(m_window, &m_mouse_x, &m_mouse_y);

	if (!m_vulkanHandler.checkUIWindowHovered()) {

	int width, height;
	glfwGetWindowSize(m_window, &width, &height);

	if (m_mouse_control) {
		

		if (m_firstClick) {
			glfwSetCursorPos(m_window, (width / 2), (height / 2));
			m_firstClick = false;
			m_mouse_x = width / 2;
			m_mouse_y = height / 2;
		}

		m_camera.update(width, height, (width / 2) - (float)m_mouse_x , (height / 2) - (float)m_mouse_y, m_deltaZoom);

		glfwSetCursorPos(m_window, (width / 2), (height / 2));
	}
	else {
		m_camera.update(width, height, 0, 0, m_deltaZoom);
		m_firstClick = true;
	}

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

