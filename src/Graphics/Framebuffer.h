#pragma once

#include <glad/glad.h>

// -----------------------------------------------------------------------------
// Framebuffer (FBO) ¡ª Off-screen render target for the dockable Viewport
// -----------------------------------------------------------------------------

class Framebuffer
{
public:
	// Default constructor (no allocation)
	Framebuffer();

	// Construct and immediately allocate GPU storage
	Framebuffer(int width, int height);

	~Framebuffer();

	// Initialize or reinitialize render targets
	void Initialize(int width, int height);

	// Resize when viewport size changes
	void Resize(int width, int height);

	// Bind / unbind framebuffer
	void Bind();
	void Unbind();

	// Access color texture for ImGui::Image
	GLuint GetColorAttachmentID() const { return m_ColorAttachment; }

	int GetWidth()  const { return m_Width; }
	int GetHeight() const { return m_Height; }

private:
	// Destroy + recreate FBO & attachments
	void Invalidate();

private:
	GLuint m_FBO = 0;
	GLuint m_ColorAttachment = 0;
	GLuint m_DepthRBO = 0;

	int m_Width = 0;
	int m_Height = 0;
};
