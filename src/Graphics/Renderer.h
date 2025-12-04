#pragma once

#include "Scene/Scene.h"
#include "Graphics/Shader.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	// Set viewport size (updated every frame by Application)
	void SetViewportSize(int width, int height);

	// Render scene using current viewport size
	void Render(const Scene& scene);

private:
	// Camera parameters
	void SetupCamera(const Camera& camera, Shader& shader, float aspectRatio);

	// Light parameters
	void SetupLights(const std::vector<Light>& lights, Shader& shader);

	// Draw a single entity
	void DrawEntity(const Entity& entity, Shader& shader);

private:
	Shader* m_DefaultShader;

	// Current viewport dimensions
	int m_ViewportWidth = 1280;
	int m_ViewportHeight = 720;
};
