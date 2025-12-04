#include "Texture.h"
#include "Utils/Log.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& filePath)
{
	stbi_set_flip_vertically_on_load(true); // flip for OpenGL UV convention

	unsigned char* data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_Channels, 0);

	if (!data)
	{
		Log::Error("Failed to load texture: " + filePath);
		return;
	}

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	// Texture filtering & wrapping settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLenum format = GL_RGB;
	if (m_Channels == 4) format = GL_RGBA;

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		m_Width,
		m_Height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		data
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	Log::Info("Texture loaded: " + filePath);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::GetWidth() const
{
	return m_Width;
}

int Texture::GetHeight() const
{
	return m_Height;
}
