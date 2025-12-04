#include "InspectorPanel.h"
#include "imgui.h"

//==============================================================
// static storage
//==============================================================
bool InspectorPanel::s_FPSRequested = false;

//==============================================================
// External access
//==============================================================
bool InspectorPanel::IsFPSModeRequested()
{
	return s_FPSRequested;
}

//==============================================================
// NEW ¡ª setter implementation so controller can update UI
//==============================================================
void InspectorPanel::SetFPSModeRequested(bool enable)
{
	s_FPSRequested = enable;
}

//--------------------------------------------------------------
// Draw entry
//--------------------------------------------------------------
void InspectorPanel::Draw(Scene& scene)
{
	if (ImGui::Begin("Inspector"))
	{
		// FPS mode toggle ¡ª bound directly to static state
		ImGui::Checkbox("Enable FPS Camera Mode", &s_FPSRequested);
		ImGui::Separator();

		DrawCameraProperties(scene.GetCamera());
		ImGui::Separator();

		DrawLightProperties(scene.GetLights());
		ImGui::Separator();

		DrawEntityProperties(scene.GetEntities());
	}
	ImGui::End();
}

//--------------------------------------------------------------
// Camera controls
//--------------------------------------------------------------
void InspectorPanel::DrawCameraProperties(Camera& camera)
{
	if (ImGui::TreeNode("Camera"))
	{
		glm::vec3 pos = camera.GetPosition();
		glm::vec3 rot = camera.GetRotation();

		// position
		if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
			camera.SetPosition(pos);

		// rotation
		if (ImGui::DragFloat3("Rotation", &rot.x, 0.1f))
			camera.SetRotation(rot);

		// FOV
		float fov = camera.GetFOV();
		if (ImGui::SliderFloat("FOV", &fov, 1.0f, 120.0f))
			camera.SetFOV(fov);

		// clipping planes
		float nearClip = camera.GetNearClip();
		float farClip = camera.GetFarClip();

		if (ImGui::DragFloat("Near Clip", &nearClip, 0.01f, 0.01f, farClip - 0.1f))
			camera.SetClippingPlanes(nearClip, farClip);

		if (ImGui::DragFloat("Far Clip", &farClip, 0.5f, nearClip + 0.1f, 500.0f))
			camera.SetClippingPlanes(nearClip, farClip);

		ImGui::TreePop();
	}
}

//--------------------------------------------------------------
// Light inspector
//--------------------------------------------------------------
void InspectorPanel::DrawLightProperties(std::vector<Light>& lights)
{
	if (ImGui::TreeNode("Lights"))
	{
		for (size_t i = 0; i < lights.size(); i++)
		{
			Light& light = lights[i];

			std::string label = "Light " + std::to_string(i);
			if (ImGui::TreeNode(label.c_str()))
			{
				glm::vec3 color = light.GetColor();
				if (ImGui::ColorEdit3("Color", &color.x))
					light.SetColor(color);

				float intensity = light.GetIntensity();
				if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 10.0f))
					light.SetIntensity(intensity);

				if (light.GetType() == LightType::Point)
				{
					glm::vec3 pos = light.GetPosition();
					if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
						light.SetPosition(pos);
				}

				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

//--------------------------------------------------------------
// Entity + Transform + Material
//--------------------------------------------------------------
void InspectorPanel::DrawEntityProperties(std::vector<Entity>& entities)
{
	if (ImGui::TreeNode("Entities"))
	{
		for (size_t i = 0; i < entities.size(); i++)
		{
			Entity& e = entities[i];
			std::string header = "Entity " + std::to_string(i);

			if (ImGui::TreeNode(header.c_str()))
			{
				//------------------------------------------------------
				// Transform
				//------------------------------------------------------
				glm::vec3 pos = e.GetTransform().GetPosition();
				glm::vec3 rot = e.GetTransform().GetRotation();
				glm::vec3 scl = e.GetTransform().GetScale();

				if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
					e.GetTransform().SetPosition(pos);

				if (ImGui::DragFloat3("Rotation", &rot.x, 0.1f))
					e.GetTransform().SetRotation(rot);

				if (ImGui::DragFloat3("Scale", &scl.x, 0.1f))
					e.GetTransform().SetScale(scl);

				//------------------------------------------------------
				// Material properties
				//------------------------------------------------------
				Material* mat = e.GetMaterial();
				glm::vec3 diffuse = mat->GetDiffuseColor();
				glm::vec3 specular = mat->GetSpecularColor();
				float shininess = mat->GetShininess();

				if (ImGui::ColorEdit3("Diffuse Color", &diffuse.x))
					mat->SetDiffuseColor(diffuse);

				if (ImGui::ColorEdit3("Specular Color", &specular.x))
					mat->SetSpecularColor(specular);

				if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 128.0f))
					mat->SetShininess(shininess);

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}
