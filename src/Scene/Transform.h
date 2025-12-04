#pragma once
#include <glm/glm.hpp>

class Transform
{
public:
	Transform();

	// Setters and Getters
	void SetPosition(const glm::vec3& pos);
	void SetRotation(const glm::vec3& rotEulerDeg);
	void SetScale(const glm::vec3& scale);

	const glm::vec3& GetPosition() const;
	const glm::vec3& GetRotation() const;
	const glm::vec3& GetScale()     const;

	glm::mat4 GetMatrix() const;    // Éú³É world transform matrix

private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;   // Euler angles in degrees
	glm::vec3 m_Scale;
};
