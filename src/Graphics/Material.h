#pragma once

#include <glm/glm.hpp>
#include "Texture.h"
#include "Shader.h"

class Material
{
public:
	Material();

	// Diffuse color
	void SetDiffuseColor(const glm::vec3& color);
	const glm::vec3& GetDiffuseColor() const;

	// Specular color
	void SetSpecularColor(const glm::vec3& color);
	const glm::vec3& GetSpecularColor() const;

	// Shading hardness
	void SetShininess(float shininess);
	float GetShininess() const;

	// Diffuse texture
	void SetDiffuseTexture(Texture* texture);
	Texture* GetDiffuseTexture() const;

	// Normal map texture
	void SetNormalMap(Texture* texture);
	Texture* GetNormalMap() const;

	// Upload material parameters to shader
	void Apply(Shader& shader) const;

private:
	glm::vec3 m_DiffuseColor = glm::vec3(1.0f);
	glm::vec3 m_SpecularColor = glm::vec3(1.0f);
	float     m_Shininess = 32.0f;

	Texture* m_DiffuseTexture = nullptr;
	Texture* m_NormalMap = nullptr;
};
