#pragma once
#include <string>

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	int GetWidth() const;
	int GetHeight() const;
	const std::string& GetPath() const { return m_FilePath; }

private:
	unsigned int m_RendererID = 0;   // OpenGL texture ID
	int m_Width = 0;
	int m_Height = 0;
	int m_Channels = 0;
	std::string m_FilePath;
};
