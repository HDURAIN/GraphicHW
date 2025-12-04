#include "Input.h"
#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(int key)
{
	auto window = glfwGetCurrentContext();
	if (!window) return false;

	return glfwGetKey((GLFWwindow*)window, key) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(int button)
{
	auto window = glfwGetCurrentContext();
	if (!window) return false;

	return glfwGetMouseButton((GLFWwindow*)window, button) == GLFW_PRESS;
}

void Input::GetMousePosition(float& xpos, float& ypos)
{
	auto window = glfwGetCurrentContext();
	if (!window)
	{
		xpos = ypos = 0.0f;
		return;
	}

	double x, y;
	glfwGetCursorPos((GLFWwindow*)window, &x, &y);

	xpos = (float)x;
	ypos = (float)y;
}
