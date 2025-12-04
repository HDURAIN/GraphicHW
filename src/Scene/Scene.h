#pragma once
#include <vector>

#include "Entity.h"
#include "Graphics/Camera.h"
#include "Graphics/Light.h"

class Scene
{
public:
	Scene();

	// Entity management
	Entity& CreateEntity(Mesh* mesh, Material* material);
	std::vector<Entity>& GetEntities();
	const std::vector<Entity>& GetEntities() const;

	// Light management
	void AddLight(const Light& light);
	std::vector<Light>& GetLights();
	const std::vector<Light>& GetLights() const;

	// Camera
	Camera& GetCamera();
	const Camera& GetCamera() const;

private:
	Camera              m_Camera;
	std::vector<Light>  m_Lights;
	std::vector<Entity> m_Entities;
};
