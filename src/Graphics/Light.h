#pragma once

#include <glm/glm.hpp>

enum class LightType
{
	Directional,
	Point
};

class Light
{
public:
	Light(LightType type);

	// Type access
	LightType GetType() const;

	// Common parameters
	void SetColor(const glm::vec3& color);
	const glm::vec3& GetColor() const;

	// Point light only
	void SetPosition(const glm::vec3& pos);
	const glm::vec3& GetPosition() const;

	// Strength/intensity for UI tuning
	void SetIntensity(float intensity);
	float GetIntensity() const;

private:
	LightType  m_Type;

	glm::vec3  m_Color = glm::vec3(1.0f);
	glm::vec3  m_Position = glm::vec3(0.0f); // used by Point light

	float      m_Intensity = 1.0f;
};
