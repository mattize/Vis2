#include "Camera.h"

void Camera::update(int width, int height, float deltaMouseX, float deltaMouseY, float zoom) {
    m_radius += zoom;
    m_radius = glm::clamp(m_radius, 1.0f, 100.0f);

    glm::vec4 position(m_position, 1.0f);
    glm::vec4 pivot(m_lookAt, 1.0f);

    double PI = std::numbers::pi;
    float deltaAngleX = (2 * PI / width); 
    float deltaAngleY = (PI / height); 
    float xAngle = deltaMouseX * deltaAngleX;
    float yAngle = deltaMouseY * deltaAngleY;

    glm::mat4x4 rotationMatrixX(1.0f);
    rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, m_up);
    position = (rotationMatrixX * (position - pivot)) + pivot;

    glm::mat4x4 rotationMatrixY(1.0f);
    rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, getRightVector());
    glm::vec3 finalPosition = (rotationMatrixY * (position - pivot)) + pivot;
 
    float pitch = glm::degrees(glm::acos(dot(glm::normalize(finalPosition), m_up)));
    if ((pitch < 179.0f) && (pitch > 1.0f)) {
        m_position = finalPosition;
    }
    else {
        m_position = position;
    }

    m_view = glm::lookAt(glm::normalize(finalPosition) * m_radius, m_lookAt, m_up);
    m_proj = glm::perspective(glm::radians(m_fov), m_aspect, m_nearPlane, m_farPlane);
}
