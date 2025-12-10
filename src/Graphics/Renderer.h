#pragma once

#include "Scene/Scene.h"
#include "Graphics/Shader.h"

// Forward declaration -- defined in Graphics/Framebuffer.h
class Framebuffer;

class Renderer
{
public:
	Renderer();
	~Renderer();

	// ------------------------------------------------------------
	// Assign an off-screen framebuffer
	// Called once by Application::Init()
	// ------------------------------------------------------------
	void SetFramebuffer(Framebuffer* framebuffer);
	Framebuffer* GetFramebuffer() const { return m_Framebuffer; }

	// ------------------------------------------------------------
	// Still kept for UI code that may call it.
	// ------------------------------------------------------------
	void SetViewportSize(int width, int height);

	// ------------------------------------------------------------
	// Render the scene into the assigned framebuffer (off-screen)
	// ------------------------------------------------------------
	void Render(const Scene& scene);

private:
	// Internal helpers
	void SetupCamera(const Camera& camera, Shader& shader, float aspectRatio);
	void SetupLights(const std::vector<Light>& lights, Shader& shader);
	void DrawEntity(const Entity& entity, Shader& shader);

private:
	Shader* m_DefaultShader = nullptr;

	// NEW in Task10 ¡ª all rendering happens into this FBO
	Framebuffer* m_Framebuffer = nullptr;

	// Legacy screen-dependent viewport settings
	int m_ViewportWidth = 1280;
	int m_ViewportHeight = 720;
};
