#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();

	// View / Projection API
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(float aspectRatio) const;

	// Position
	void SetPosition(const glm::vec3& pos);
	const glm::vec3& GetPosition() const;

	// Orientation (Euler degrees)
	void SetRotation(const glm::vec3& rotEulerDeg);
	const glm::vec3& GetRotation() const;

	// Projection
	void SetFOV(float fovY);
	float GetFOV() const;

	void SetClippingPlanes(float nearPlane, float farPlane);
	float GetNearClip() const;
	float GetFarClip() const;

private:
	// Default values ensure scene visibility on startup
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 5.0f);   // Default camera position
	glm::vec3 m_Rotation = glm::vec3(0.0f, -90.0f, 0.0f); // Facing -Z

	float m_FOV = 45.0f;
	float m_NearClip = 0.1f;
	float m_FarClip = 100.0f;
};
