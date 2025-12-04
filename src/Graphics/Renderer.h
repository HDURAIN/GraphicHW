#pragma once

#include "Scene/Scene.h"
#include "Graphics/Shader.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	// Render scene
	void Render(const Scene& scene);

private:
	// Camera parameters
	void SetupCamera(const Camera& camera, Shader& shader);

	// Light parameters
	void SetupLights(const std::vector<Light>& lights, Shader& shader);

	// Draw a single entity
	void DrawEntity(const Entity& entity, Shader& shader);

private:
	Shader* m_DefaultShader; // Default Phong or normal-mapped shader
};
