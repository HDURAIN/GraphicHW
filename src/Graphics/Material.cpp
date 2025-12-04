#include "Material.h"

Material::Material()
{
}

void Material::SetDiffuseColor(const glm::vec3& color)
{
	m_DiffuseColor = color;
}

const glm::vec3& Material::GetDiffuseColor() const
{
	return m_DiffuseColor;
}

void Material::SetSpecularColor(const glm::vec3& color)
{
	m_SpecularColor = color;
}

const glm::vec3& Material::GetSpecularColor() const
{
	return m_SpecularColor;
}

void Material::SetShininess(float shininess)
{
	m_Shininess = shininess;
}

float Material::GetShininess() const
{
	return m_Shininess;
}

void Material::SetDiffuseTexture(Texture* texture)
{
	m_DiffuseTexture = texture;
}

Texture* Material::GetDiffuseTexture() const
{
	return m_DiffuseTexture;
}

void Material::SetNormalMap(Texture* texture)
{
	m_NormalMap = texture;
}

Texture* Material::GetNormalMap() const
{
	return m_NormalMap;
}

// Apply material parameters and bind textures
void Material::Apply(Shader& shader) const
{
	shader.Bind();

	shader.SetVec3("u_Material.diffuseColor", m_DiffuseColor);
	shader.SetVec3("u_Material.specularColor", m_SpecularColor);
	shader.SetFloat("u_Material.shininess", m_Shininess);

	// Diffuse texture (slot 0)
	if (m_DiffuseTexture)
	{
		m_DiffuseTexture->Bind(0);
		shader.SetInt("u_Material.diffuseTexture", 0);
	}
	else
	{
		shader.SetInt("u_Material.diffuseTexture", -1);
	}

	// Normal map (slot 1)
	if (m_NormalMap)
	{
		m_NormalMap->Bind(1);
		shader.SetInt("u_NormalMap", 1);
		shader.SetInt("u_NormalMapEnabled", 1);
	}
	else
	{
		shader.SetInt("u_NormalMapEnabled", 0);
	}
}
