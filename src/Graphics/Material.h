#pragma once

#include <glm/glm.hpp>
#include "Texture.h"
#include "Shader.h"

class Material
{
public:
	Material();

	// Base color used when no albedo texture is assigned
	void SetDiffuseColor(const glm::vec3& color);
	const glm::vec3& GetDiffuseColor() const;

	// Specular color (legacy field ¡ª still used by UI but not PBR shader)
	void SetSpecularColor(const glm::vec3& color);
	const glm::vec3& GetSpecularColor() const;

	// Shininess (legacy field ¡ª still editable but unused by PBR shader)
	void SetShininess(float shininess);
	float GetShininess() const;

	// Albedo texture (PBR base color map)
	void SetDiffuseTexture(Texture* texture);
	Texture* GetDiffuseTexture() const;

	// Normal map (TBN-normal perturbation)
	void SetNormalMap(Texture* texture);
	Texture* GetNormalMap() const;

	// Roughness map (microfacet distribution)
	void SetRoughnessMap(Texture* texture);
	Texture* GetRoughnessMap() const;

	// Metalness map (F0 metallness factor)
	void SetMetalnessMap(Texture* texture);
	Texture* GetMetalnessMap() const;

	// Displacement / Height map (parallax mapping)
	void SetDisplacementMap(Texture* texture);
	Texture* GetDisplacementMap() const;

	// Duplicate this material object
	Material* Clone() const;

	// Apply all active textures and fallback values to the GPU shader
	void Apply(Shader& shader) const;

private:
	// Default PBR albedo if no texture is assigned
	glm::vec3 m_DiffuseColor = glm::vec3(0.8f);

	// Legacy Phong params (retained for compatibility/UI)
	glm::vec3 m_SpecularColor = glm::vec3(1.0f);
	float     m_Shininess = 32.0f;

	// Texture bindings
	Texture* m_DiffuseTexture = nullptr;
	Texture* m_NormalMap = nullptr;
	Texture* m_RoughnessMap = nullptr;
	Texture* m_MetalnessMap = nullptr;
	Texture* m_DisplacementMap = nullptr;
};
