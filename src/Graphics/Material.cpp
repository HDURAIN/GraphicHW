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

//---------------------------------------------------------
// Apply() — upload uniforms and bind textures
//---------------------------------------------------------
void Material::Apply(Shader& shader) const
{
	shader.Bind();

	shader.SetVec3("u_Material.diffuseColor", m_DiffuseColor);
	shader.SetVec3("u_Material.specularColor", m_SpecularColor);
	shader.SetFloat("u_Material.shininess", m_Shininess);

	if (m_DiffuseTexture)
	{
		m_DiffuseTexture->Bind(0);
		shader.SetInt("u_Material.diffuseTexture", 0);
	}
	else
	{
		// 无纹理时禁用 sampling
		shader.SetInt("u_Material.diffuseTexture", -1);
	}
}
