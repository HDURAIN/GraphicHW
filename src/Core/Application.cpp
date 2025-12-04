#include "Application.h"
#include "Utils/Log.h"
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"
#include "Graphics/Light.h"
#include <GLFW/glfw3.h>

//---------------------------------------------------------
// Constructor ¡ª camera controller now initialized here
//---------------------------------------------------------
Application::Application()
	: m_Window(1280, 720, "GraphicHW")
	, m_UI(&m_Window)
	, m_CamController(&m_Scene.GetCamera(), &m_Window)
{
	Init();
}

Application::~Application()
{
	Shutdown();
}

//---------------------------------------------------------
// Init system + scene
//---------------------------------------------------------
void Application::Init()
{
	Log::Info("Initializing Application...");

	// 1) Lights
	Light dirLight(LightType::Directional);
	dirLight.SetColor({ 1.0f, 1.0f, 1.0f });
	dirLight.SetIntensity(1.0f);
	m_Scene.AddLight(dirLight);

	Light pointLight(LightType::Point);
	pointLight.SetPosition({ 2.0f, 2.0f, 2.0f });
	pointLight.SetColor({ 1.0f, 0.95f, 0.95f });
	pointLight.SetIntensity(2.0f);
	m_Scene.AddLight(pointLight);

	// 2) Mesh + material
	Mesh* cubeMesh = Mesh::CreateCube();
	Material* cubeMat = new Material();
	cubeMat->SetDiffuseColor({ 0.6f, 0.6f, 0.8f });

	// 3) Sample entities ¡ª positioned in camera view
	const float spacing = 2.0f;
	for (int i = 0; i < 3; i++)
	{
		Entity& e = m_Scene.CreateEntity(cubeMesh, cubeMat);
		e.GetTransform().SetPosition({ (i - 1) * spacing, 0.0f, 0.0f });
	}
}

//---------------------------------------------------------
// Shutdown
//---------------------------------------------------------
void Application::Shutdown()
{
	Log::Info("Shutting down Application...");
}

//---------------------------------------------------------
// Main loop
//---------------------------------------------------------
void Application::Run()
{
	while (!m_Window.ShouldClose() && m_Running)
	{
		// 1) Time update
		m_Timer.Update();
		float dt = m_Timer.GetDeltaTime();

		// 2) Poll input and system events
		m_Window.PollEvents();

		// ESC to exit application
		if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			m_Running = false;
			continue;
		}

		// 3) UI begin frame
		m_UI.BeginFrame();

		// 4) Modify scene via UI
		m_UI.Render(m_Scene);

		// 5) FPS Camera update
		m_CamController.Update(dt);

		// ============================
		// NEW: Dynamic viewport update
		// ============================
		int winWidth = m_Window.GetWidth();
		int winHeight = m_Window.GetHeight();
		m_Renderer.SetViewportSize(winWidth, winHeight);

		// 6) World rendering
		m_Renderer.Render(m_Scene);

		// 7) UI end frame
		m_UI.EndFrame();

		// 8) Swap buffers
		m_Window.SwapBuffers();
	}
}
