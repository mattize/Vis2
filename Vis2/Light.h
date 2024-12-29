#pragma once

#include "Common.h"

class Light {
public:
	Light() {};
	Light(float intensity, glm::vec3 color, glm::vec3 position) : m_intensity(intensity), m_color(color), m_position(position) {};

	float getIntensity() const { return m_intensity; }
	glm::vec3 getColor() const { return m_color; }
	glm::vec3 getPosition() const { return m_position; }

private:
	float m_intensity;
	glm::vec3 m_color;
	glm::vec3 m_position;
};
