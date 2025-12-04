#include "Entity.h"

Entity::Entity(Mesh* mesh, Material* material)
	: m_Mesh(mesh),
	m_Material(material)
{
}

//---------------------------------------------------------
// Transform access
//---------------------------------------------------------
Transform& Entity::GetTransform()
{
	return m_Transform;
}

const Transform& Entity::GetTransform() const
{
	return m_Transform;
}

//---------------------------------------------------------
// Mesh access
//---------------------------------------------------------
Mesh* Entity::GetMesh() const
{
	return m_Mesh;
}

//---------------------------------------------------------
// Material access
//---------------------------------------------------------
Material* Entity::GetMaterial() const
{
	return m_Material;
}
