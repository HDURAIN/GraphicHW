#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

Camera::Camera()
	: m_Position(0.0f, 0.0f, 5.0f),       // Default position
	m_Rotation(0.0f, -90.0f, 0.0f),    // Face toward -Z
	m_FOV(45.0f),
	m_NearClip(0.1f),
	m_FarClip(100.0f)
{
}

//---------------------------------------------------------
// View Matrix
//---------------------------------------------------------
glm::mat4 Camera::GetViewMatrix() const
{
	float pitch = glm::radians(m_Rotation.x);
	float yaw = glm::radians(m_Rotation.y);
	float roll = glm::radians(m_Rotation.z);

	glm::vec3 front;
	front.x = cos(yaw) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(yaw) * cos(pitch);
	front = glm::normalize(front);

	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
	glm::vec3 up = glm::normalize(glm::cross(right, front));

	return glm::lookAt(m_Position, m_Position + front, up);
}

//---------------------------------------------------------
// Projection Matrix
//---------------------------------------------------------
glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const
{
	return glm::perspective(glm::radians(m_FOV), aspectRatio, m_NearClip, m_FarClip);
}

//---------------------------------------------------------
// Position control
//---------------------------------------------------------
void Camera::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
}

const glm::vec3& Camera::GetPosition() const
{
	return m_Position;
}

//---------------------------------------------------------
// Rotation control
//---------------------------------------------------------
void Camera::SetRotation(const glm::vec3& rotEulerDeg)
{
	m_Rotation = rotEulerDeg;
}

const glm::vec3& Camera::GetRotation() const
{
	return m_Rotation;
}

//---------------------------------------------------------
// Projection parameters
//---------------------------------------------------------
void Camera::SetFOV(float fovY)
{
	m_FOV = fovY;
}

float Camera::GetFOV() const
{
	return m_FOV;
}

void Camera::SetClippingPlanes(float nearPlane, float farPlane)
{
	m_NearClip = nearPlane;
	m_FarClip = farPlane;
}

float Camera::GetNearClip() const
{
	return m_NearClip;
}

float Camera::GetFarClip() const
{
	return m_FarClip;
}
