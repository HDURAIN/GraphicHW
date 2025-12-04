#pragma once

#include <vector>
#include <glm/glm.hpp>

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;
		glm::vec3 Tangent;
	};

public:
	Mesh(const std::vector<Vertex>& vertices,
		const std::vector<unsigned int>& indices);

	Mesh(const std::vector<glm::vec3>& positions,
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec2>& uvs,
		const std::vector<unsigned int>& indices);

	~Mesh();

	void Bind() const;
	void Draw() const;

	static Mesh* CreateCube();

	void RecalculateTangents();

private:
	void UploadToGPU();

private:
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	unsigned int m_EBO = 0;

	unsigned int m_IndexCount = 0;

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
};
