#pragma once

#include "Window.h"
#include "Timer.h"
#include "Scene/Scene.h"
#include "UI/UIManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Framebuffer.h"       // бя NEW for Task10
#include "Controller/CameraController.h"

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	void Init();
	void Shutdown();

private:
	Window           m_Window;
	Timer            m_Timer;
	Scene            m_Scene;

	UIManager        m_UI;
	Renderer         m_Renderer;

	Framebuffer* m_Framebuffer = nullptr;   // бя NEW: Off-screen render target

	CameraController m_CamController;

	bool             m_Running = true;
};
