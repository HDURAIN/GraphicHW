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

void Material::SetRoughnessMap(Texture* texture)
{
	m_RoughnessMap = texture;
}

Texture* Material::GetRoughnessMap() const
{
	return m_RoughnessMap;
}

void Material::SetMetalnessMap(Texture* texture)
{
	m_MetalnessMap = texture;
}

Texture* Material::GetMetalnessMap() const
{
	return m_MetalnessMap;
}

void Material::SetDisplacementMap(Texture* texture)
{
	m_DisplacementMap = texture;
}

Texture* Material::GetDisplacementMap() const
{
	return m_DisplacementMap;
}

Material* Material::Clone() const
{
	return new Material(*this);
}

// ============================================================
// Apply material parameters and bind textures
// ============================================================
void Material::Apply(Shader& shader) const
{
	shader.Bind();

	// ============================================================
	// PBR fallback value uniforms (ALL VALID & USED IN SHADER)
	// ============================================================
	shader.SetVec3("u_Material_DefaultAlbedo", m_DiffuseColor);
	shader.SetFloat("u_DefaultRoughness", 0.5f);
	shader.SetFloat("u_DefaultMetalness", 0.0f);

	// ============================================================
	// Texture Binding Layout:
	//   0 = Albedo
	//   1 = Normal
	//   2 = Roughness
	//   3 = Metalness
	//   4 = Displacement
	// ============================================================

	// Albedo
	if (m_DiffuseTexture)
	{
		m_DiffuseTexture->Bind(0);
		shader.SetInt("u_AlbedoMap", 0);
		shader.SetInt("u_AlbedoMapEnabled", 1);
	}
	else
	{
		shader.SetInt("u_AlbedoMapEnabled", 0);
	}

	// Normal
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

	// Roughness
	if (m_RoughnessMap)
	{
		m_RoughnessMap->Bind(2);
		shader.SetInt("u_RoughnessMap", 2);
		shader.SetInt("u_RoughnessMapEnabled", 1);
	}
	else
	{
		shader.SetInt("u_RoughnessMapEnabled", 0);
	}

	// Metalness
	if (m_MetalnessMap)
	{
		m_MetalnessMap->Bind(3);
		shader.SetInt("u_MetalnessMap", 3);
		shader.SetInt("u_MetalnessMapEnabled", 1);
	}
	else
	{
		shader.SetInt("u_MetalnessMapEnabled", 0);
	}

	// Displacement
	if (m_DisplacementMap)
	{
		m_DisplacementMap->Bind(4);
		shader.SetInt("u_DisplacementMap", 4);
		shader.SetInt("u_DisplacementMapEnabled", 1);
	}
	else
	{
		shader.SetInt("u_DisplacementMapEnabled", 0);
	}
}
