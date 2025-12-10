#include "Application.h"
#include "Utils/Log.h"
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"
#include "Graphics/Light.h"
#include "Graphics/Framebuffer.h"
#include <GLFW/glfw3.h>

//---------------------------------------------------------
// Constructor ¡ª camera controller initialized here
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

	// =====================================================
	// 1) Create off-screen framebuffer
	// =====================================================
	m_Framebuffer = new Framebuffer(1280, 720);
	m_Renderer.SetFramebuffer(m_Framebuffer);

	// =====================================================
	// 2) Lighting
	// =====================================================
	Light dirLight(LightType::Directional);
	dirLight.SetColor({ 1.0f, 1.0f, 1.0f });
	dirLight.SetIntensity(3.0f);
	m_Scene.AddLight(dirLight);

	Light pointLight(LightType::Point);
	pointLight.SetPosition({ 2.0f, 2.0f, 2.0f });
	pointLight.SetColor({ 1.0f, 0.95f, 0.95f });
	pointLight.SetIntensity(5.0f);
	pointLight.SetPosition({ 0.0f, 2.5f, 2.0f });
	m_Scene.AddLight(pointLight);

	// =====================================================
	// 3) Mesh + Material
	// =====================================================
	Mesh* cubeMesh = Mesh::CreateCube();
	Material* cubeMat = new Material();
	cubeMat->SetDiffuseColor({ 0.6f, 0.6f, 0.8f });

	// =====================================================
	// 4) Create sample entities
	// =====================================================
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

	delete m_Framebuffer;
	m_Framebuffer = nullptr;
}

void Application::UpdateEntityAnimations(float dt)
{
	for (auto& e : m_Scene.GetEntities())
	{
		glm::vec3 rot = e.GetTransform().GetRotation();

		rot.x += 20.0f * dt;
		rot.y += 30.0f * dt;
		rot.z += 15.0f * dt;

		e.GetTransform().SetRotation(rot);
	}
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

		// 2) Poll input
		m_Window.PollEvents();

		// ESC exits application
		if (Input::IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			m_Running = false;
			continue;
		}

		// 3) Begin UI frame
		m_UI.BeginFrame();

		// 4) UI modifies scene
		m_UI.Render(m_Scene, m_Renderer);

		// 5) FPS camera update
		m_CamController.Update(dt);

		// 6) Auto-rotation animation
		 UpdateEntityAnimations(dt);

		// 7) Render world into Framebuffer (NOT to screen)
		m_Renderer.Render(m_Scene);

		// 8) End UI frame (ImGui draws to screen)
		m_UI.EndFrame();

		// 9) Present
		m_Window.SwapBuffers();
	}
}
