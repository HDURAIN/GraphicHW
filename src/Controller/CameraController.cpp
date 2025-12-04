#include "CameraController.h"
#include "UI/InspectorPanel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

CameraController::CameraController(Camera* camera, Window* window)
	: m_Camera(camera), m_Window(window)
{
}

bool CameraController::IsFPSMode() const
{
	return m_IsFPS;
}

// ------------------------------------------------------------
// Enable/disable FPS mode
// ------------------------------------------------------------
void CameraController::EnableFPS(bool enable)
{
	m_IsFPS = enable;

	if (enable)
	{
		// Reset mouse reference and sync with current cursor
		m_FirstMouse = true;

		// Lock cursor for FPS view
		m_Window->SetCursorLocked(true);
	}
	else
	{
		m_Window->SetCursorLocked(false);
	}
}

// ------------------------------------------------------------
// Main per-frame update
// ------------------------------------------------------------
void CameraController::Update(float dt)
{
	// 1) Desired state from UI
	bool desired = InspectorPanel::IsFPSModeRequested();

	// 2) Override with F1
	if (Input::IsKeyPressed(GLFW_KEY_F1))
	{
		desired = false;
		InspectorPanel::SetFPSModeRequested(false);
	}

	// 3) If changed, apply new state
	if (desired != m_IsFPS)
	{
		EnableFPS(desired);
	}

	if (!m_IsFPS)
		return;

	ProcessMovement(dt);
	ProcessMouseLook(dt);
}

// ------------------------------------------------------------
// WASD movement
// ------------------------------------------------------------
void CameraController::ProcessMovement(float dt)
{
	glm::vec3 pos = m_Camera->GetPosition();
	glm::vec3 rot = m_Camera->GetRotation();

	float speed = m_MoveSpeed * dt;

	float yaw = glm::radians(rot.y);
	float pitch = glm::radians(rot.x);

	glm::vec3 front(
		cos(yaw) * cos(pitch),
		sin(pitch),
		sin(yaw) * cos(pitch)
	);
	front = glm::normalize(front);

	glm::vec3 worldUp(0, 1, 0);
	glm::vec3 right = glm::normalize(glm::cross(front, worldUp));

	if (Input::IsKeyPressed('W')) pos += front * speed;
	if (Input::IsKeyPressed('S')) pos -= front * speed;
	if (Input::IsKeyPressed('A')) pos -= right * speed;
	if (Input::IsKeyPressed('D')) pos += right * speed;
	if (Input::IsKeyPressed(GLFW_KEY_SPACE)) pos.y += speed;
	if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) pos.y -= speed;

	m_Camera->SetPosition(pos);
}

// ------------------------------------------------------------
// Mouse look
// ------------------------------------------------------------
void CameraController::ProcessMouseLook(float dt)
{
	float xpos, ypos;
	Input::GetMousePosition(xpos, ypos);

	// First mouse event: just store cur position, do NOT rotate
	if (m_FirstMouse)
	{
		m_LastMouseX = xpos;
		m_LastMouseY = ypos;
		m_FirstMouse = false;
		return;  // Prevents first-frame jump
	}

	float dx = xpos - m_LastMouseX;
	float dy = m_LastMouseY - ypos;

	m_LastMouseX = xpos;
	m_LastMouseY = ypos;

	dx *= m_MouseSensitivity;
	dy *= m_MouseSensitivity;

	glm::vec3 rot = m_Camera->GetRotation();
	rot.y += dx;
	rot.x += dy;

	rot.x = glm::clamp(rot.x, -89.0f, 89.0f);
	m_Camera->SetRotation(rot);
}
