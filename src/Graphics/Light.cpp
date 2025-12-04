#include "Light.h"

Light::Light(LightType type)
	: m_Type(type)
{
}

LightType Light::GetType() const
{
	return m_Type;
}

void Light::SetColor(const glm::vec3& color)
{
	m_Color = color;
}

const glm::vec3& Light::GetColor() const
{
	return m_Color;
}

void Light::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
}

const glm::vec3& Light::GetPosition() const
{
	return m_Position;
}

void Light::SetIntensity(float intensity)
{
	m_Intensity = intensity;
}

float Light::GetIntensity() const
{
	return m_Intensity;
}
