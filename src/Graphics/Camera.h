#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();

	// View / Projection API
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(float aspectRatio) const;

	// Position setters/getters
	void SetPosition(const glm::vec3& pos);
	const glm::vec3& GetPosition() const;

	// Orientation setters/getters (Euler angles in degrees)
	void SetRotation(const glm::vec3& rotEulerDeg);
	const glm::vec3& GetRotation() const;

	// Projection parameter control
	void SetFOV(float fovY);
	float GetFOV() const;

	void SetClippingPlanes(float nearPlane, float farPlane);
	float GetNearClip() const;
	float GetFarClip() const;

private:
	// Stored values
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;      // Euler angles, pitch/yaw/roll (degrees)

	float m_FOV;
	float m_NearClip;
	float m_FarClip;
};
