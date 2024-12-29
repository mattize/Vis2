#pragma once

#include "Common.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

#include <numbers>

class Camera {
public:
	Camera() {};
	Camera(uint32_t width, uint32_t height, float fov, float nearPlane, float farPlane) : WIDTH(width), HEIGHT(height), m_fov(fov), m_nearPlane(nearPlane), m_farPlane(farPlane) {};

	void update(int width, int height, float deltaMouseX, float deltaMouseY, float zoom);

	glm::mat4 getProjMat() const { return m_proj; };
	glm::mat4 getViewMat() const { return m_view; };
	glm::vec3 getPosition() const { return m_position; };

private:

	glm::vec3 getViewDir() const { return -glm::transpose(m_view)[2]; }
	glm::vec3 getRightVector() const { return glm::transpose(m_view)[0]; }

	glm::mat4 m_proj = glm::mat4(1.0f);
	glm::mat4 m_view = glm::mat4(1.0f);
	bool m_debug = false;

	uint32_t WIDTH;
	uint32_t HEIGHT;

	float m_fov;
	float m_aspect = (float)WIDTH / (float)HEIGHT;
	float m_nearPlane;
	float m_farPlane;

	bool m_firstClick = true;
	float m_sensitivity = 160.0f;
	float m_radius = 6.0f;

	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 m_lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_speed = 0.05f;
};