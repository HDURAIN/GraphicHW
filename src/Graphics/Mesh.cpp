#include "Mesh.h"
#include <glad/glad.h>

// Construct from ready vertex buffer
Mesh::Mesh(const std::vector<Vertex>& vertices,
	const std::vector<unsigned int>& indices)
	: m_Vertices(vertices), m_Indices(indices)
{
	m_IndexCount = static_cast<unsigned int>(indices.size());
	RecalculateTangents();
	UploadToGPU();
}

// Legacy constructor (positions + normals + uvs)
Mesh::Mesh(const std::vector<glm::vec3>& positions,
	const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec2>& uvs,
	const std::vector<unsigned int>& indices)
{
	m_Vertices.reserve(positions.size());
	for (size_t i = 0; i < positions.size(); i++)
	{
		Vertex v;
		v.Position = positions[i];
		v.Normal = normals[i];
		v.UV = uvs[i];
		v.Tangent = glm::vec3(0.0f);
		m_Vertices.push_back(v);
	}

	m_Indices = indices;
	m_IndexCount = static_cast<unsigned int>(indices.size());

	RecalculateTangents();
	UploadToGPU();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_EBO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::Bind() const
{
	glBindVertexArray(m_VAO);
}

void Mesh::Draw() const
{
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
}

// Compute per-vertex tangents from triangle data
void Mesh::RecalculateTangents()
{
	for (auto& v : m_Vertices)
		v.Tangent = glm::vec3(0.0f);

	for (size_t i = 0; i < m_Indices.size(); i += 3)
	{
		Vertex& v0 = m_Vertices[m_Indices[i + 0]];
		Vertex& v1 = m_Vertices[m_Indices[i + 1]];
		Vertex& v2 = m_Vertices[m_Indices[i + 2]];

		glm::vec3 e1 = v1.Position - v0.Position;
		glm::vec3 e2 = v2.Position - v0.Position;

		glm::vec2 d1 = v1.UV - v0.UV;
		glm::vec2 d2 = v2.UV - v0.UV;

		float r = 1.0f;
		float det = d1.x * d2.y - d1.y * d2.x;
		if (det != 0.0f)
			r = 1.0f / det;

		glm::vec3 t = (e1 * d2.y - e2 * d1.y) * r;

		v0.Tangent += t;
		v1.Tangent += t;
		v2.Tangent += t;
	}

	for (auto& v : m_Vertices)
		v.Tangent = glm::normalize(v.Tangent);
}

// Upload vertex attributes and index buffer
void Mesh::UploadToGPU()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex),
		m_Vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int),
		m_Indices.data(), GL_STATIC_DRAW);

	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, Position));

	// Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, Normal));

	// UV
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, UV));

	// Tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, Tangent));

	glBindVertexArray(0);
}

// Create cube with normal mapping support
Mesh* Mesh::CreateCube()
{
	std::vector<Vertex> v;
	std::vector<unsigned int> idx;

	auto PushFace = [&](glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d,
		glm::vec3 normal)
		{
			unsigned int start = v.size();

			v.push_back({ a, normal, {0,0}, glm::vec3(0) });
			v.push_back({ b, normal, {1,0}, glm::vec3(0) });
			v.push_back({ c, normal, {1,1}, glm::vec3(0) });
			v.push_back({ d, normal, {0,1}, glm::vec3(0) });

			idx.push_back(start + 0);
			idx.push_back(start + 1);
			idx.push_back(start + 2);
			idx.push_back(start + 2);
			idx.push_back(start + 3);
			idx.push_back(start + 0);
		};

	PushFace({ -0.5,-0.5,-0.5 }, { 0.5,-0.5,-0.5 }, { 0.5,0.5,-0.5 }, { -0.5,0.5,-0.5 }, { 0,0,-1 });
	PushFace({ -0.5,-0.5, 0.5 }, { 0.5,-0.5, 0.5 }, { 0.5,0.5, 0.5 }, { -0.5,0.5, 0.5 }, { 0,0, 1 });
	PushFace({ -0.5,-0.5,-0.5 }, { -0.5,-0.5, 0.5 }, { -0.5,0.5, 0.5 }, { -0.5,0.5,-0.5 }, { -1,0,0 });
	PushFace({ 0.5,-0.5,-0.5 }, { 0.5,-0.5, 0.5 }, { 0.5,0.5, 0.5 }, { 0.5,0.5,-0.5 }, { 1,0,0 });
	PushFace({ -0.5,0.5,-0.5 }, { 0.5,0.5,-0.5 }, { 0.5,0.5, 0.5 }, { -0.5,0.5, 0.5 }, { 0,1,0 });
	PushFace({ -0.5,-0.5,-0.5 }, { 0.5,-0.5,-0.5 }, { 0.5,-0.5, 0.5 }, { -0.5,-0.5, 0.5 }, { 0,-1,0 });

	return new Mesh(v, idx);
}
