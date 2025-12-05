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
	// 即使开启 Docking，我们这里也不再使用 DockSpace / DockBuilder
	// 只是让窗口本身可以被停靠（如有需要）
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Load readable font
	io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Medium.ttf", 20.0f);
	if (io.Fonts->Fonts.empty())
		io.Fonts->AddFontDefault();

	ImGui::StyleColorsDark();

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
// BeginDockspace
// 现在保留空实现，仅为兼容 UIManager.h 的声明
// 不再创建 DockSpace，也不再使用 DockBuilder 系列 API
// ------------------------------------------------------------
void UIManager::BeginDockspace()
{
	// 不做任何事，布局改由一个主窗口 + 子区域完成
}

// ------------------------------------------------------------
// BeginFrame
// ------------------------------------------------------------
void UIManager::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 不再创建 DockSpace
	BeginDockspace();
}

// ------------------------------------------------------------
// Draw the real-time Scene Viewport (right side)
// ------------------------------------------------------------
void UIManager::DrawViewport(Renderer& renderer)
{
	// 这里假设我们已经在一个“右侧子区域”中
	// 所以只负责根据当前可用区域大小去调整 Framebuffer 并画 Image

	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	Log::Info("Viewport size: " + std::to_string((int)viewportSize.x)
		+ "x" + std::to_string((int)viewportSize.y));

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
			ImVec2(0, 1), ImVec2(1, 0) // Flip vertically
		);
	}
	else
	{
		ImGui::Text("No framebuffer assigned to Renderer.");
	}
}

// ------------------------------------------------------------
// Render UI Panels (Inspector + Viewport)
// 使用一个“全屏主窗口” + 左右两个子区域来布局
// ------------------------------------------------------------
void UIManager::Render(Scene& scene, Renderer& renderer)
{
	ImGuiIO& io = ImGui::GetIO();

	// 创建一个覆盖整个应用窗口的主窗口
	ImGuiWindowFlags mainFlags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus;

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);

	ImGui::Begin("MainUI", nullptr, mainFlags);

	// 左侧 Inspector 区域宽度（固定像素，或按比例）
	float leftWidth = io.DisplaySize.x * 0.30f;
	if (leftWidth < 250.0f)
		leftWidth = 250.0f;

	// ---------------- 左侧：Inspector ----------------
	ImGui::BeginChild("InspectorRegion",
		ImVec2(leftWidth, 0.0f),     // 高度 0 表示自动填满主窗口高度
		true,                        // 边框
		ImGuiWindowFlags_HorizontalScrollbar);

	m_InspectorPanel.Draw(scene);
	ImGui::EndChild();

	// ---------------- 右侧：Viewport ----------------
	ImGui::SameLine();

	ImGui::BeginChild("ViewportRegion",
		ImVec2(0.0f, 0.0f),          // 剩余所有宽度和高度
		true,
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	DrawViewport(renderer);

	ImGui::EndChild();

	ImGui::End(); // MainUI

	// 生成渲染数据
	ImGui::Render();
}

// ------------------------------------------------------------
// EndFrame
// ------------------------------------------------------------
void UIManager::EndFrame()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
