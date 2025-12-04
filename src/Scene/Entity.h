#pragma once
#include "Transform.h"
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"

class Entity
{
public:
	Entity(Mesh* mesh, Material* material);

	Transform& GetTransform();
	const Transform& GetTransform() const;

	Mesh* GetMesh() const;
	Material* GetMaterial() const;

private:
	Transform  m_Transform;
	Mesh* m_Mesh;       // shared pointer-like usage
	Material* m_Material;   // shared across entities possible
};
