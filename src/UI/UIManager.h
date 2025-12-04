#pragma once

#include "Core/Window.h"
#include "Scene/Scene.h"
#include "InspectorPanel.h"

class UIManager
{
public:
	UIManager(Window* window);
	~UIManager();

	void BeginFrame();                // ImGui::NewFrame + backend new frame
	void Render(Scene& scene);        // 调用面板绘制，并执行 ImGui::Render()
	void EndFrame();                  // 渲染 ImGui 数据 + swap buffer 若需要

private:
	InspectorPanel m_InspectorPanel;
};
