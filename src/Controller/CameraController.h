#pragma once

#include "Graphics/Camera.h"
#include "Core/Input.h"
#include "Core/Window.h"

class CameraController
{
public:
	CameraController(Camera* camera, Window* window);

	// Per-frame update
	void Update(float deltaTime);

	bool IsFPSMode() const;
	void EnableFPS(bool enable);

private:
	void ProcessMovement(float dt);
	void ProcessMouseLook(float dt);

private:
	Camera* m_Camera;
	Window* m_Window;

	bool  m_IsFPS = false;
	float m_MoveSpeed = 3.0f;
	float m_MouseSensitivity = 0.1f;

	bool  m_FirstMouse = true;
	float m_LastMouseX = 0.0f;
	float m_LastMouseY = 0.0f;
};
