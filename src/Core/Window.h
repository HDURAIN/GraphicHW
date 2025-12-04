#pragma once
#include <string>

class Window
{
public:
	Window(int width = 1280, int height = 720, const std::string& title = "GraphicHW");
	~Window();

	void PollEvents();    // 处理输入系统/窗口消息
	void SwapBuffers();   // 渲染后交换 buffer

	bool ShouldClose() const;

	int  GetWidth() const;
	int  GetHeight() const;

	void* GetNativeHandle() const;   // 返回 GLFWwindow* 用于 imgui/renderer

	// NEW —— 鼠标锁定/自由模式切换
	void SetCursorLocked(bool locked);

private:
	struct Impl;
	Impl* m_Impl;  // PIMPL 隐藏 GLFW 细节
};
