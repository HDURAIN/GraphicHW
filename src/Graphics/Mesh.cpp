#include "Mesh.h"
#include <glad/glad.h>

//--------------------------------------------------------
// 构造：上传 vertices + indices 到 GPU
//--------------------------------------------------------
Mesh::Mesh(const std::vector<glm::vec3>& positions,
	const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec2>& uvs,
	const std::vector<unsigned int>& indices)
{
	m_IndexCount = static_cast<unsigned int>(indices.size());

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	std::vector<Vertex> vertices;
	vertices.reserve(positions.size());

	for (size_t i = 0; i < positions.size(); i++)
		vertices.push_back({ positions[i], normals[i], uvs[i] });

	// GPU 资源创建
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	// vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// layout
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}

//--------------------------------------------------------
// 析构
//--------------------------------------------------------
Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_EBO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
}

//--------------------------------------------------------
// 绑定/绘制
//--------------------------------------------------------
void Mesh::Bind() const { glBindVertexArray(m_VAO); }

void Mesh::Draw() const
{
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
}

//--------------------------------------------------------
// 真实法线版 cube primitive
//--------------------------------------------------------
Mesh* Mesh::CreateCube()
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;

	auto PushFace = [&](glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec3 normal)
		{
			unsigned int start = positions.size();

			positions.push_back(a); normals.push_back(normal); uvs.push_back({ 0,0 });
			positions.push_back(b); normals.push_back(normal); uvs.push_back({ 1,0 });
			positions.push_back(c); normals.push_back(normal); uvs.push_back({ 1,1 });
			positions.push_back(d); normals.push_back(normal); uvs.push_back({ 0,1 });

			indices.push_back(start + 0);
			indices.push_back(start + 1);
			indices.push_back(start + 2);
			indices.push_back(start + 2);
			indices.push_back(start + 3);
			indices.push_back(start + 0);
		};

	// 6 faces with real normals
	PushFace({ -0.5,-0.5,-0.5 }, { 0.5,-0.5,-0.5 }, { 0.5,0.5,-0.5 }, { -0.5,0.5,-0.5 }, { 0,0,-1 });
	PushFace({ -0.5,-0.5,0.5 }, { 0.5,-0.5,0.5 }, { 0.5,0.5,0.5 }, { -0.5,0.5,0.5 }, { 0,0,1 });
	PushFace({ -0.5,-0.5,-0.5 }, { -0.5,-0.5,0.5 }, { -0.5,0.5,0.5 }, { -0.5,0.5,-0.5 }, { -1,0,0 });
	PushFace({ 0.5,-0.5,-0.5 }, { 0.5,-0.5,0.5 }, { 0.5,0.5,0.5 }, { 0.5,0.5,-0.5 }, { 1,0,0 });
	PushFace({ -0.5,0.5,-0.5 }, { 0.5,0.5,-0.5 }, { 0.5,0.5,0.5 }, { -0.5,0.5,0.5 }, { 0,1,0 });
	PushFace({ -0.5,-0.5,-0.5 }, { 0.5,-0.5,-0.5 }, { 0.5,-0.5,0.5 }, { -0.5,-0.5,0.5 }, { 0,-1,0 });

	return new Mesh(positions, normals, uvs, indices);
}
