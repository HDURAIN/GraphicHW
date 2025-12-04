#pragma once

#include <glm/glm.hpp>
#include "Texture.h"
#include "Shader.h"

class Material
{
public:
	Material();

	// 颜色参数
	void SetDiffuseColor(const glm::vec3& color);
	const glm::vec3& GetDiffuseColor() const;

	void SetSpecularColor(const glm::vec3& color);
	const glm::vec3& GetSpecularColor() const;

	void SetShininess(float shininess);
	float GetShininess() const;

	// 纹理（可选绑定）
	void SetDiffuseTexture(Texture* texture);
	Texture* GetDiffuseTexture() const;

	// 上传 uniform 给 shader
	void Apply(Shader& shader) const;

private:
	glm::vec3 m_DiffuseColor = glm::vec3(1.0f);
	glm::vec3 m_SpecularColor = glm::vec3(1.0f);
	float     m_Shininess = 32.0f;

	Texture* m_DiffuseTexture = nullptr;
};
