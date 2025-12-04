#include "Window.h"
#include "Utils/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Window::Impl
{
	GLFWwindow* handle = nullptr;
	int width;
	int height;
	std::string title;
};

Window::Window(int width, int height, const std::string& title)
{
	m_Impl = new Impl();
	m_Impl->width = width;
	m_Impl->height = height;
	m_Impl->title = title;

	// Init GLFW first time
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

	// Make OpenGL context active
	glfwMakeContextCurrent(m_Impl->handle);

	// Load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Log::Error("Failed to initialize GLAD!");
		return;
	}

	Log::Info("Window created successfully: " + title);
}

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

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_Impl->handle);
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_Impl->handle);
}

int Window::GetWidth() const
{
	return m_Impl->width;
}

int Window::GetHeight() const
{
	return m_Impl->height;
}

void* Window::GetNativeHandle() const
{
	return m_Impl->handle;
}

//============================================================
// NEW ¡ª Cursor Lock Support
//============================================================
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
