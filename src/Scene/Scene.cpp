#include "Scene.h"

Scene::Scene()
	: m_Camera()
{
}

//---------------------------------------------------------
// Entity creation ¡ª clone material to ensure independence
//---------------------------------------------------------
Entity& Scene::CreateEntity(Mesh* mesh, Material* material)
{
	m_Entities.emplace_back(mesh, material->Clone());
	return m_Entities.back();
}

//---------------------------------------------------------
// Entity accessors
//---------------------------------------------------------
std::vector<Entity>& Scene::GetEntities()
{
	return m_Entities;
}

const std::vector<Entity>& Scene::GetEntities() const
{
	return m_Entities;
}

//---------------------------------------------------------
// Light management
//---------------------------------------------------------
void Scene::AddLight(const Light& light)
{
	m_Lights.push_back(light);
}

std::vector<Light>& Scene::GetLights()
{
	return m_Lights;
}

const std::vector<Light>& Scene::GetLights() const
{
	return m_Lights;
}

//---------------------------------------------------------
// Camera access
//---------------------------------------------------------
Camera& Scene::GetCamera()
{
	return m_Camera;
}

const Camera& Scene::GetCamera() const
{
	return m_Camera;
}
