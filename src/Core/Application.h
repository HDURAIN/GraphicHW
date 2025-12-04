#pragma once

#include "Window.h"
#include "Timer.h"
#include "Scene/Scene.h"
#include "UI/UIManager.h"
#include "Graphics/Renderer.h"
#include "Controller/CameraController.h"   // NEW

class Application
{
public:
	Application();
	~Application();

	void Run();    // 主循环入口

private:
	void Init();   // 初始化全系统依赖
	void Shutdown();

private:
	Window           m_Window;
	Timer            m_Timer;
	Scene            m_Scene;
	UIManager        m_UI;
	Renderer         m_Renderer;

	CameraController m_CamController;   // NEW

	bool             m_Running = true;
};
