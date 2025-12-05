#include "Framebuffer.h"
#include "Utils/Log.h"

#include <glad/glad.h>

// ------------------------------------------------------------
// Default constructor (no GPU resource yet)
// ------------------------------------------------------------
Framebuffer::Framebuffer()
{
}

// ------------------------------------------------------------
// Construct + initialize
// ------------------------------------------------------------
Framebuffer::Framebuffer(int width, int height)
	: m_Width(width), m_Height(height)
{
	Initialize(width, height);
}

// ------------------------------------------------------------
// Destructor ¡ª cleanup GPU resources
// ------------------------------------------------------------
Framebuffer::~Framebuffer()
{
	if (m_ColorAttachment)
		glDeleteTextures(1, &m_ColorAttachment);

	if (m_DepthRBO)
		glDeleteRenderbuffers(1, &m_DepthRBO);

	if (m_FBO)
		glDeleteFramebuffers(1, &m_FBO);
}

// ------------------------------------------------------------
// Initialize framebuffer (first time)
// ------------------------------------------------------------
void Framebuffer::Initialize(int width, int height)
{
	m_Width = width;
	m_Height = height;

	Invalidate();
}

// ------------------------------------------------------------
// Resize framebuffer (called when Viewport size changes)
// ------------------------------------------------------------
void Framebuffer::Resize(int width, int height)
{
	if (width == 0 || height == 0)
		return;

	if (width == m_Width && height == m_Height)
		return;

	m_Width = width;
	m_Height = height;

	Invalidate();
}

// ------------------------------------------------------------
// Bind framebuffer
// ------------------------------------------------------------
void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

// ------------------------------------------------------------
// Unbind ¡ú back to default framebuffer
// ------------------------------------------------------------
void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// ------------------------------------------------------------
// (Re)create FBO, attachments, depth buffer
// ------------------------------------------------------------
void Framebuffer::Invalidate()
{
	// Delete previous attachments if any
	if (m_FBO)
		glDeleteFramebuffers(1, &m_FBO);
	if (m_ColorAttachment)
		glDeleteTextures(1, &m_ColorAttachment);
	if (m_DepthRBO)
		glDeleteRenderbuffers(1, &m_DepthRBO);

	// --------------------------------------------------------
	// Generate new FBO
	// --------------------------------------------------------
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// --------------------------------------------------------
	// Create Color Texture Attachment
	// --------------------------------------------------------
	glGenTextures(1, &m_ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		m_Width, m_Height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		nullptr
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		m_ColorAttachment,
		0
	);

	// --------------------------------------------------------
	// Create Depth + Stencil Renderbuffer
	// --------------------------------------------------------
	glGenRenderbuffers(1, &m_DepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
	glRenderbufferStorage(
		GL_RENDERBUFFER,
		GL_DEPTH24_STENCIL8,
		m_Width, m_Height
	);

	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		m_DepthRBO
	);

	// --------------------------------------------------------
	// Check FBO completeness
	// --------------------------------------------------------
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Log::Error("Framebuffer incomplete!");
	}
	else
	{
		Log::Info("Framebuffer created: " +
			std::to_string(m_Width) + "x" +
			std::to_string(m_Height));
	}

	// --------------------------------------------------------
	// Unbind ¡ª finished
	// --------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
