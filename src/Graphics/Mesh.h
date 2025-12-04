#pragma once

#include <vector>
#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh(const std::vector<glm::vec3>& positions,
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec2>& uvs,
		const std::vector<unsigned int>& indices);

	~Mesh();

	void Bind() const;
	void Draw() const;

	// 工厂方法：创建立方体 primitive
	static Mesh* CreateCube();

private:
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	unsigned int m_EBO = 0;

	unsigned int m_IndexCount = 0;
};
