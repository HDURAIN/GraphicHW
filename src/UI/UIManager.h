#pragma once

#include "Core/Window.h"
#include "Scene/Scene.h"
#include "InspectorPanel.h"

class Renderer;      // Forward declare
class Framebuffer;   // Forward declare

class UIManager
{
public:
	UIManager(Window* window);
	~UIManager();

	// NewFrame
	void BeginFrame();

	// Draw all UI panels (Inspector + Viewport)
	void Render(Scene& scene, Renderer& renderer);

	// Render draw data
	void EndFrame();

private:
	// DockSpace window host
	void BeginDockspace();

	// Draws the real-time render viewport using the framebuffer's color texture
	void DrawViewport(Renderer& renderer);

private:
	InspectorPanel m_InspectorPanel;
	Window* m_Window = nullptr;

	// Dockspace always visible for our engine
	bool m_ShowDockspace = true;

	// Track last viewport size (used to resize framebuffer)
	int m_ViewportWidth = 0;
	int m_ViewportHeight = 0;
};
