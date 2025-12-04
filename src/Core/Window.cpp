#include "Window.h"
#include "Utils/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ============================================================================
// Impl struct definition
// ============================================================================
struct Window::Impl
{
	GLFWwindow* handle = nullptr;
	int width;
	int height;
	std::string title;
};

// ============================================================================
// Framebuffer Resize Callback
// ============================================================================
void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	// Retrieve our Impl instance from GLFW user pointer
	void* ptr = glfwGetWindowUserPointer(window);
	if (!ptr)
		return;

	auto* impl = reinterpret_cast<Window::Impl*>(ptr);

	impl->width = width;
	impl->height = height;
}

// ============================================================================
// Constructor
// ============================================================================
Window::Window(int width, int height, const std::string& title)
{
	m_Impl = new Impl();
	m_Impl->width = width;
	m_Impl->height = height;
	m_Impl->title = title;

	if (!glfwInit())
	{
		Log::Error("Failed to initialize GLFW!");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	m_Impl->handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!m_Impl->handle)
	{
		Log::Error("Failed to create GLFW window!");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Impl->handle);

	// Register user pointer & framebuffer resize callback
	glfwSetWindowUserPointer(m_Impl->handle, m_Impl);
	glfwSetFramebufferSizeCallback(m_Impl->handle, Window::FramebufferResizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Log::Error("Failed to initialize GLAD!");
		return;
	}

	Log::Info("Window created successfully: " + title);
}

// ============================================================================
// Destructor
// ============================================================================
Window::~Window()
{
	if (m_Impl)
	{
		if (m_Impl->handle)
		{
			glfwDestroyWindow(m_Impl->handle);
			Log::Info("Window destroyed");
		}

		glfwTerminate();
		delete m_Impl;
	}
}

// ============================================================================
// Poll events
// ============================================================================
void Window::PollEvents()
{
	glfwPollEvents();
}

// ============================================================================
// Swap buffers
// ============================================================================
void Window::SwapBuffers()
{
	glfwSwapBuffers(m_Impl->handle);
}

// ============================================================================
// Should close?
// ============================================================================
bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_Impl->handle);
}

// ============================================================================
// Dynamic window size getters (updated by callback)
// ============================================================================
int Window::GetWidth() const
{
	return m_Impl->width;
}

int Window::GetHeight() const
{
	return m_Impl->height;
}

// ============================================================================
// Native handle getter
// ============================================================================
void* Window::GetNativeHandle() const
{
	return m_Impl->handle;
}

// ============================================================================
// Cursor lock feature
// ============================================================================
void Window::SetCursorLocked(bool locked)
{
	if (!m_Impl || !m_Impl->handle)
		return;

	if (locked)
	{
		glfwSetInputMode(m_Impl->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Log::Info("Cursor locked (FPS mode enabled)");
	}
	else
	{
		glfwSetInputMode(m_Impl->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		Log::Info("Cursor unlocked (UI mode enabled)");
	}
}
