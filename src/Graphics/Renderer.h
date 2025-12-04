#pragma once

#include "Scene/Scene.h"
#include "Graphics/Shader.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	// 渲染入口
	void Render(const Scene& scene);

private:
	// 渲染流程拆解辅助函数（内部用）
	void SetupCamera(const Camera& camera, Shader& shader);
	void SetupLights(const std::vector<Light>& lights, Shader& shader);
	void DrawEntity(const Entity& entity, Shader& shader);

private:
	Shader* m_DefaultShader; // 可支持一个基本Phong shader
};
