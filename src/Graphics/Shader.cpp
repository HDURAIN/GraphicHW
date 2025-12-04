#include "Shader.h"
#include "Utils/FileSystem.h"
#include "Utils/Log.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
		Log::Error(std::string("Shader Compilation Failed: ") + infoLog);
	}

	return shader;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexSrc = FileSystem::ReadFile(vertexPath);
	std::string fragmentSrc = FileSystem::ReadFile(fragmentPath);

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

	m_RendererID = glCreateProgram();
	glAttachShader(m_RendererID, vs);
	glAttachShader(m_RendererID, fs);
	glLinkProgram(m_RendererID);

	int success;
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetProgramInfoLog(m_RendererID, 1024, nullptr, infoLog);
		Log::Error(std::string("Shader Linking Failed: ") + infoLog);
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	Log::Info("Shader created: " + vertexPath);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

int Shader::GetUniformLocation(const std::string& name) const
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		Log::Warn("Uniform '" + name + "' does not exist or is unused.");
	return location;
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(GetUniformLocation(name), value);
}
