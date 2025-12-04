#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

Transform::Transform()
	: m_Position(0.0f),
	m_Rotation(0.0f),
	m_Scale(1.0f)
{
}

//---------------------------------------------------------
// Setters
//---------------------------------------------------------
void Transform::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
}

void Transform::SetRotation(const glm::vec3& rotEulerDeg)
{
	m_Rotation = rotEulerDeg;
}

void Transform::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
}

//---------------------------------------------------------
// Getters
//---------------------------------------------------------
const glm::vec3& Transform::GetPosition() const
{
	return m_Position;
}

const glm::vec3& Transform::GetRotation() const
{
	return m_Rotation;
}

const glm::vec3& Transform::GetScale() const
{
	return m_Scale;
}

//---------------------------------------------------------
// World Matrix
//---------------------------------------------------------
glm::mat4 Transform::GetMatrix() const
{
	glm::mat4 model(1.0f);

	// translation
	model = glm::translate(model, m_Position);

	// Euler rotation
	model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0)); // pitch
	model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0)); // yaw
	model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1)); // roll

	// scaling
	model = glm::scale(model, m_Scale);

	return model;
}
