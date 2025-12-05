#include "UIManager.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Core/Window.h"
#include "Graphics/Renderer.h"
#include "Graphics/Framebuffer.h"
#include "Utils/Log.h"

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
UIManager::UIManager(Window* window)
	: m_InspectorPanel(), m_Window(window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // 启用 docking

	// Load readable font
	io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Medium.ttf", 20.0f);
	if (io.Fonts->Fonts.empty())
		io.Fonts->AddFontDefault();

	ImGui::StyleColorsLight();

	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetNativeHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	m_ShowDockspace = true;
}

// ------------------------------------------------------------
// Destructor
// ------------------------------------------------------------
UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

// ------------------------------------------------------------
// DockSpace host（使用 DockSpaceOverViewport，无 DockBuilder）
// ------------------------------------------------------------
void UIManager::BeginDockspace()
{
	ImGuiIO& io = ImGui::GetIO();
	if (!(io.ConfigFlags & ImGuiConfigFlags_DockingEnable))
		return;

	ImGuiViewport* vp = ImGui::GetMainViewport();

	// 旧版 DockSpaceOverViewport API：参数 1 = ID, 参数 2 = Viewport*
	ImGui::DockSpaceOverViewport(0, vp);
}

// ------------------------------------------------------------
// BeginFrame
// ------------------------------------------------------------
void UIManager::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 全屏 DockSpace
	BeginDockspace();
}

// ------------------------------------------------------------
// Draw the real-time Scene Viewport
// ------------------------------------------------------------
void UIManager::DrawViewport(Renderer& renderer)
{
	// 注意：位置和大小由外部 ImGui::Begin("Viewport") 窗口控制
	// 这里只负责根据可用区域大小调整 FBO，并绘制图像。

	ImVec2 viewportSize = ImGui::GetContentRegionAvail();

	int newW = (int)viewportSize.x;
	int newH = (int)viewportSize.y;

	// Resize framebuffer only if size changed
	if (newW > 0 && newH > 0 &&
		(newW != m_ViewportWidth || newH != m_ViewportHeight))
	{
		m_ViewportWidth = newW;
		m_ViewportHeight = newH;

		if (renderer.GetFramebuffer())
			renderer.GetFramebuffer()->Resize(newW, newH);
	}

	// Display the framebuffer texture
	if (renderer.GetFramebuffer())
	{
		ImTextureID texID = (ImTextureID)(intptr_t)
			renderer.GetFramebuffer()->GetColorAttachmentID();

		ImGui::Image(
			texID,
			viewportSize,
			ImVec2(0, 1), ImVec2(1, 0) // 垂直翻转
		);
	}
	else
	{
		ImGui::Text("No framebuffer assigned to Renderer.");
	}
}

// ------------------------------------------------------------
// Render UI Panels (Inspector + Viewport)
// 使用两个普通可 Dock 的窗口（无 DockBuilder）
// ------------------------------------------------------------
void UIManager::Render(Scene& scene, Renderer& renderer)
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 displaySize = io.DisplaySize;

	// ============================
	// Inspector 窗口（左侧首开布局）
	// ============================
	float leftWidth = displaySize.x * 0.30f;
	if (leftWidth < 250.0f)
		leftWidth = 250.0f;

	ImGui::SetNextWindowSize(ImVec2(leftWidth, displaySize.y), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);

	ImGui::Begin("Inspector");
	m_InspectorPanel.Draw(scene);
	ImGui::End();

	// ============================
	// Viewport 窗口（右侧首开布局）
	// ============================
	ImGui::SetNextWindowSize(
		ImVec2(displaySize.x - leftWidth, displaySize.y),
		ImGuiCond_FirstUseEver
	);
	ImGui::SetNextWindowPos(
		ImVec2(leftWidth, 0.0f),
		ImGuiCond_FirstUseEver
	);

	ImGui::Begin("Viewport");
	DrawViewport(renderer);
	ImGui::End();

	// 生成 ImGui 绘制数据
	ImGui::Render();
}

// ------------------------------------------------------------
// EndFrame
// ------------------------------------------------------------
void UIManager::EndFrame()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
