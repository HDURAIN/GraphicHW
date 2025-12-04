#pragma once
#include <string>

struct GLFWwindow;   // Forward declare to avoid including GLFW in header

class Window
{
public:
	Window(int width = 1280, int height = 720, const std::string& title = "GraphicHW");
	~Window();

	void PollEvents();
	void SwapBuffers();
	bool ShouldClose() const;

	int  GetWidth() const;
	int  GetHeight() const;

	void* GetNativeHandle() const;

	// Mouse lock feature
	void SetCursorLocked(bool locked);

private:
	// Forward-declared implementation struct
	struct Impl;
	Impl* m_Impl;

	// Framebuffer resize callback (must match GLFW callback signature)
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
};
