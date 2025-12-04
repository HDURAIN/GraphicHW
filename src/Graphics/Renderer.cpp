#include "Renderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils/Log.h"

Renderer::Renderer()
{
	// Load default Phong shader
	m_DefaultShader = new Shader("shaders/triangle.vert", "shaders/triangle.frag");

	// Enable depth testing (fix correct 3D layering)
	glEnable(GL_DEPTH_TEST);

	// No face culling (your requirement)
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);

	// Initialize texture sampler binding
	m_DefaultShader->Bind();
	m_DefaultShader->SetInt("u_TextureSampler", 0);
}

Renderer::~Renderer()
{
	delete m_DefaultShader;
}

//---------------------------------------------------------
// Camera uniform setup
//---------------------------------------------------------
void Renderer::SetupCamera(const Camera& camera, Shader& shader)
{
	glm::mat4 view = camera.GetViewMatrix();

	// NOTE: Renderer needs aspect ratio ¡ª using fixed 16:9 for now
	glm::mat4 projection = camera.GetProjectionMatrix(16.0f / 9.0f);

	shader.SetMat4("u_View", view);
	shader.SetMat4("u_Projection", projection);
}

//---------------------------------------------------------
// Light uniform setup
//---------------------------------------------------------
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

//---------------------------------------------------------
// Render single entity
//---------------------------------------------------------
void Renderer::DrawEntity(const Entity& entity, Shader& shader)
{
	// Upload material properties
	entity.GetMaterial()->Apply(shader);

	// Upload model matrix
	glm::mat4 model = entity.GetTransform().GetMatrix();
	shader.SetMat4("u_Model", model);

	// Draw mesh
	entity.GetMesh()->Bind();
	entity.GetMesh()->Draw();
}

//---------------------------------------------------------
// Main render entry
//---------------------------------------------------------
void Renderer::Render(const Scene& scene)
{
	// Clear buffer every frame (fix ghosting)
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader& shader = *m_DefaultShader;
	shader.Bind();

	// Setup camera
	SetupCamera(scene.GetCamera(), shader);

	// Upload light data
	SetupLights(scene.GetLights(), shader);

	// Render entities
	for (const auto& entity : scene.GetEntities())
	{
		DrawEntity(entity, shader);
	}

	shader.Unbind();
}
