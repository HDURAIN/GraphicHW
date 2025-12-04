#include "Timer.h"
#include <GLFW/glfw3.h>

Timer::Timer()
{
	m_LastTime = glfwGetTime();
	m_DeltaTime = 0.0f;
}

void Timer::Update()
{
	float currentTime = glfwGetTime();
	m_DeltaTime = currentTime - m_LastTime;
	m_LastTime = currentTime;
}

float Timer::GetDeltaTime()
{
	return m_DeltaTime;
}
