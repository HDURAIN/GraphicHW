#include "Renderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils/Log.h"

Renderer::Renderer()
{
	m_DefaultShader = new Shader("shaders/triangle.vert", "shaders/triangle.frag");

	glEnable(GL_DEPTH_TEST);

	m_DefaultShader->Bind();
	m_DefaultShader->SetInt("u_TextureSampler", 0);

	m_ViewportWidth = 1280;
	m_ViewportHeight = 720;
}

Renderer::~Renderer()
{
	delete m_DefaultShader;
}

// ------------------------------------------------------------
// Viewport update
// ------------------------------------------------------------
void Renderer::SetViewportSize(int width, int height)
{
	if (width > 0 && height > 0)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}
}

// ------------------------------------------------------------
// Camera setup (now using dynamic aspect ratio)
// ------------------------------------------------------------
void Renderer::SetupCamera(const Camera& camera, Shader& shader, float aspectRatio)
{
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);

	shader.SetMat4("u_View", view);
	shader.SetMat4("u_Projection", projection);
}

// ------------------------------------------------------------
// Light uniform setup (NO direction upload!)
// ------------------------------------------------------------
void Renderer::SetupLights(const std::vector<Light>& lights, Shader& shader)
{
	int dirCount = 0;
	int pointCount = 0;

	for (size_t i = 0; i < lights.size(); i++)
	{
		const Light& light = lights[i];

		if (light.GetType() == LightType::Directional)
		{
			shader.SetVec3("u_DirectionalLight.color", light.GetColor());
			shader.SetFloat("u_DirectionalLight.intensity", light.GetIntensity());
			dirCount++;
		}
		else if (light.GetType() == LightType::Point)
		{
			std::string base = "u_PointLights[" + std::to_string(pointCount) + "]";
			shader.SetVec3(base + ".position", light.GetPosition());
			shader.SetVec3(base + ".color", light.GetColor());
			shader.SetFloat(base + ".intensity", light.GetIntensity());
			pointCount++;
		}
	}

	shader.SetInt("u_PointLightCount", pointCount);
	shader.SetInt("u_HasDirectionalLight", dirCount > 0 ? 1 : 0);
}

// ------------------------------------------------------------
// Draw entity
// ------------------------------------------------------------
void Renderer::DrawEntity(const Entity& entity, Shader& shader)
{
	entity.GetMaterial()->Apply(shader);

	glm::mat4 model = entity.GetTransform().GetMatrix();
	shader.SetMat4("u_Model", model);

	entity.GetMesh()->Bind();
	entity.GetMesh()->Draw();
}

// ------------------------------------------------------------
// Render scene (viewport-aware)
// ------------------------------------------------------------
void Renderer::Render(const Scene& scene)
{
	glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader& shader = *m_DefaultShader;
	shader.Bind();

	float aspect = (float)m_ViewportWidth / (float)m_ViewportHeight;
	SetupCamera(scene.GetCamera(), shader, aspect);

	SetupLights(scene.GetLights(), shader);

	for (const auto& entity : scene.GetEntities())
		DrawEntity(entity, shader);

	shader.Unbind();
}
