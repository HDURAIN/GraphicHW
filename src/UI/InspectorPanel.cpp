#include "InspectorPanel.h"
#include "imgui.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureLibrary.h"
#include "Utils/FileSystem.h"

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

		if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
			camera.SetPosition(pos);

		if (ImGui::DragFloat3("Rotation", &rot.x, 0.1f))
			camera.SetRotation(rot);

		float fov = camera.GetFOV();
		if (ImGui::SliderFloat("FOV", &fov, 1.0f, 120.0f))
			camera.SetFOV(fov);

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
// Lights
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
// Entity + Material + Texture Maps
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
				// Material
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

				//------------------------------------------------------
				// Texture Maps
				//------------------------------------------------------
				if (ImGui::TreeNode("Texture Maps"))
				{
					// Load available textures
					std::vector<std::string> fileList =
						FileSystem::ListFiles("assets/textures");

					bool hasTextures = !fileList.empty();

					// Add "<None>"
					fileList.insert(fileList.begin(), "<None>");

					// char* list
					std::vector<const char*> names;
					names.reserve(fileList.size());
					for (auto& s : fileList)
						names.push_back(s.c_str());

					if (!hasTextures)
					{
						ImGui::TextColored(ImVec4(1, 0.6f, 0.4f, 1),
							"No .jpg textures found in /assets/textures/");
					}

					//--------------------------------------------------
					// Helper: extract filename from full path
					//--------------------------------------------------
					auto ExtractFileName = [&](const std::string& path) -> std::string
						{
							size_t pos = path.find_last_of("/\\");
							if (pos == std::string::npos)
								return path;
							return path.substr(pos + 1);
						};

					//--------------------------------------------------
					// Get index from material's Texture*
					//--------------------------------------------------
					auto GetIndex = [&](Texture* tex) -> int
						{
							if (!tex) return 0;
							std::string name = ExtractFileName(tex->GetPath());

							for (int j = 1; j < fileList.size(); j++)
								if (fileList[j] == name)
									return j;

							return 0;
						};

					//--------------------------------------------------
					// Generic Drop-down function
					//--------------------------------------------------
					auto DrawSelector = [&](const char* label,
						Texture* current,
						std::function<void(Texture*)> setter)
						{
							int idx = GetIndex(current);

							if (ImGui::Combo(label, &idx, names.data(), names.size()))
							{
								if (idx == 0)
									setter(nullptr);
								else
								{
									std::string fullPath =
										"./assets/textures/" + fileList[idx];
									setter(TextureLibrary::GetOrLoad(fullPath));
								}
							}
						};

					//--------------------------------------------------
					// Final UI fields
					//--------------------------------------------------
					DrawSelector("Albedo Map",
						mat->GetDiffuseTexture(),
						[&](Texture* t) { mat->SetDiffuseTexture(t); });

					DrawSelector("Normal Map",
						mat->GetNormalMap(),
						[&](Texture* t) { mat->SetNormalMap(t); });

					DrawSelector("Roughness Map",
						mat->GetRoughnessMap(),
						[&](Texture* t) { mat->SetRoughnessMap(t); });

					DrawSelector("Metalness Map",
						mat->GetMetalnessMap(),
						[&](Texture* t) { mat->SetMetalnessMap(t); });

					DrawSelector("Displacement Map",
						mat->GetDisplacementMap(),
						[&](Texture* t) { mat->SetDisplacementMap(t); });

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}
