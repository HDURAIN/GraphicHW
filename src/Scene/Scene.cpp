#include "Scene.h"

Scene::Scene()
	: m_Camera()   // camera default ctor ok
{
}

//---------------------------------------------------------
// Entity creation ¡ª returns reference for transform editing
//---------------------------------------------------------
Entity& Scene::CreateEntity(Mesh* mesh, Material* material)
{
	m_Entities.emplace_back(mesh, material);
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
