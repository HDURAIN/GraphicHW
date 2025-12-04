#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniform values
	void SetMat4(const std::string& name, const glm::mat4& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetInt(const std::string& name, int value) const;

private:
	unsigned int m_RendererID;

	int GetUniformLocation(const std::string& name) const;
};
