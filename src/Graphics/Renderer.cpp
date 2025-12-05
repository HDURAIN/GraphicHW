#include "Renderer.h"
#include "Graphics/Framebuffer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils/Log.h"

Renderer::Renderer()
{
	Log::Info("Rendering to framebuffer...");
	// Load PBR shader
	m_DefaultShader = new Shader("assets/shaders/pbr.vert",
		"assets/shaders/pbr.frag");

	glEnable(GL_DEPTH_TEST);

	m_DefaultShader->Bind();
	m_DefaultShader->Unbind();

	// Legacy viewport defaults
	m_ViewportWidth = 1280;
	m_ViewportHeight = 720;
}

Renderer::~Renderer()
{
	delete m_DefaultShader;
}

// ------------------------------------------------------------
// Assign framebuffer (Task10)
// ------------------------------------------------------------
void Renderer::SetFramebuffer(Framebuffer* framebuffer)
{
	m_Framebuffer = framebuffer;
}

// ------------------------------------------------------------
// Legacy viewport setter (no longer used by FBO rendering)
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
// Camera setup (aspect = framebuffer size)
// ------------------------------------------------------------
void Renderer::SetupCamera(const Camera& camera, Shader& shader, float aspectRatio)
{
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);

	shader.SetMat4("u_View", view);
	shader.SetMat4("u_Projection", projection);

	shader.SetVec3("u_ViewPos", camera.GetPosition());
}

// ------------------------------------------------------------
// Upload lights to shader
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
// Draw a single entity
// ------------------------------------------------------------
void Renderer::DrawEntity(const Entity& entity, Shader& shader)
{
	// Material uploads PBR texture maps + shader uniforms
	entity.GetMaterial()->Apply(shader);

	glm::mat4 model = entity.GetTransform().GetMatrix();
	shader.SetMat4("u_Model", model);

	entity.GetMesh()->Bind();
	entity.GetMesh()->Draw();
}

// ------------------------------------------------------------
// Render scene into framebuffer (NOT screen)
// ------------------------------------------------------------
void Renderer::Render(const Scene& scene)
{
	if (!m_Framebuffer)
	{
		Log::Error("Renderer::Render() called with no framebuffer assigned!");
		return;
	}

	// Bind FBO
	m_Framebuffer->Bind();

	int fbWidth = m_Framebuffer->GetWidth();
	int fbHeight = m_Framebuffer->GetHeight();

	glViewport(0, 0, fbWidth, fbHeight);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader& shader = *m_DefaultShader;
	shader.Bind();

	float aspectRatio = (float)fbWidth / (float)fbHeight;

	SetupCamera(scene.GetCamera(), shader, aspectRatio);
	SetupLights(scene.GetLights(), shader);

	for (const auto& entity : scene.GetEntities())
		DrawEntity(entity, shader);

	shader.Unbind();

	// Unbind FBO ¡ú back to screen (so ImGui can draw)
	m_Framebuffer->Unbind();
}
