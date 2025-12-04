#include "Texture.h"
#include "Utils/Log.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& filePath)
{
	// ------------------------------------------------------------
	// Save texture path so InspectorPanel can match UI states
	// ------------------------------------------------------------
	m_FilePath = filePath;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(filePath.c_str(),
		&m_Width,
		&m_Height,
		&m_Channels,
		0);

	if (!data)
	{
		Log::Error("Failed to load texture: " + filePath);
		return;
	}

	// ------------------------------------------------------------
	// Determine OpenGL texture format based on channel count
	// ------------------------------------------------------------
	GLenum format = GL_RGB;
	GLenum internalFmt = GL_RGB8;

	if (m_Channels == 1)
	{
		format = GL_RED;
		internalFmt = GL_R8;
	}
	else if (m_Channels == 3)
	{
		format = GL_RGB;
		internalFmt = GL_RGB8;
	}
	else if (m_Channels == 4)
	{
		format = GL_RGBA;
		internalFmt = GL_RGBA8;
	}
	else
	{
		Log::Warn("Unsupported texture channel count: " + std::to_string(m_Channels));
	}

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	// Filtering & wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Upload texture with correct format
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		internalFmt,
		m_Width,
		m_Height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		data
	);

	// ------------------------------------------------------------
	// Swizzle mask for single-channel textures (roughness, metalness...)
	// ------------------------------------------------------------
	if (m_Channels == 1)
	{
		GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
	}

	// Prevent crashes on grayscale textures
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	Log::Info("Texture loaded (" + std::to_string(m_Channels)
		+ " channels): " + filePath);
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

int Texture::GetWidth() const { return m_Width; }
int Texture::GetHeight() const { return m_Height; }
