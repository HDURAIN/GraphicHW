#pragma once
#include "Scene/Scene.h"

class InspectorPanel
{
public:
	InspectorPanel() = default;

	void Draw(Scene& scene);

	// Query current FPS request (used by CameraController)
	static bool IsFPSModeRequested();

	// NEW ¡ª allow external systems (CameraController) to update UI state
	static void SetFPSModeRequested(bool enable);

private:
	void DrawCameraProperties(Camera& camera);
	void DrawLightProperties(std::vector<Light>& lights);
	void DrawEntityProperties(std::vector<Entity>& entities);

private:
	// Persistent UI state for FPS checkbox
	static bool s_FPSRequested;
};
