#include "UIManager.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Core/Window.h"

UIManager::UIManager(Window* window)
	: m_InspectorPanel()
{
	// 创建 ImGui 上下文
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// 样式
	ImGui::StyleColorsDark();

	// Backend 初始化 (GLFW + OpenGL3)
	ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window->GetNativeHandle()), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UIManager::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UIManager::Render(Scene& scene)
{
	// 绘制 Scene 面板（实体列表、属性编辑）
	m_InspectorPanel.Draw(scene);

	// 生成渲染数据
	ImGui::Render();
}

void UIManager::EndFrame()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
