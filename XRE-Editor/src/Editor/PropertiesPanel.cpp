#include <ImGui\imgui.h>
#include "PropertiesPanel.h"
#include <glm\glm\gtc\type_ptr.hpp>
#include <ImGui\imgui_internal.h>

using namespace entt;
namespace XRE {
	PropertiesPanel::PropertiesPanel(ScenePanel* sp)
	{
		SetReference(sp);
	}
	void PropertiesPanel::SetReference(ScenePanel* sp)
	{
		m_ReferenceScenePanel = sp;
	}
	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin(u8"ÊôÐÔ");

		GameObject go = m_ReferenceScenePanel->m_Selected;
		if (go) {
			
			DrawComponents(go);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					if(!go.HasComponent<CameraComponent>())
					go.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Mesh Renderer"))
				{
					if (!go.HasComponent<MeshRendererComponent>())
					go.AddComponent<MeshRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Point Light"))
				{
					if (!go.HasComponent<PointLightComponent>())
					go.AddComponent<PointLightComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Directional Light"))
				{
					if (!go.HasComponent<DirectionalLightComponent>())
					go.AddComponent<DirectionalLightComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Easy Animator"))
				{
					if (!go.HasComponent<AnimatorComponent>())
					go.AddComponent<AnimatorComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
		


	}

	

	void PropertiesPanel::DrawComponents(GameObject go)
	{
		if (go.HasComponent<NameComponent>())
		{
			
		
					
				//XRE_INFO("Property find:{0}",s.value().cast<std::string>() );
				
				
				// ...
			

			auto& tag = go.GetComponent<NameComponent>().m_ObjName;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Name", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (go.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = go.GetComponent<TransformComponent>();
				DrawVec3Control("Position", transform.m_Translation);
				DrawVec3Control("Rotation", transform.m_Rotation);
				DrawVec3Control("Scale", transform.m_Scale);
				/*	ImGui::DragFloat3("Position", glm::value_ptr(transform.m_Translation), 0.1f);
					ImGui::DragFloat3("Rotation", glm::value_ptr(transform.m_Rotation), 1.0f);
					ImGui::DragFloat3("Scale", glm::value_ptr(transform.m_Scale), 0.1f);*/
				ImGui::TreePop();
			}
		}

		

		

		if (go.HasComponent<MeshRendererComponent>()) 
			DrawComponent<MeshRendererComponent>(go);

		if (go.HasComponent<AnimatorComponent>())
			DrawComponent<AnimatorComponent>(go);

		if (go.HasComponent<CameraComponent>())
			DrawComponent<CameraComponent>(go);

		if (go.HasComponent<PointLightComponent>())
			DrawComponent<PointLightComponent>(go);

		if (go.HasComponent<DirectionalLightComponent>())
			DrawComponent<DirectionalLightComponent>(go);
	}

	template<typename T>
	void PropertiesPanel::DrawComponent(GameObject go) {
		T& c = go.GetComponent<T>();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		bool open = ImGui::TreeNodeEx((void*)typeid(MeshRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, c.m_Name.c_str());
		ImGui::SameLine(ImGui::GetWindowWidth() - 120.0f);
		ImGui::Checkbox("Activate", &c.m_Active);
		ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
		if (ImGui::Button("+"))
		{
			ImGui::OpenPopup("ComponentSettings");
		}
		ImGui::PopStyleVar();
		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}
		if (open)
		{
			DrawComponentLayout<T>(c);
			ImGui::TreePop();
		}
		if (removeComponent)
			go.RemoveComponent<MeshRendererComponent>();

	}

	template<>
	void PropertiesPanel::DrawComponentLayout<MeshRendererComponent>(MeshRendererComponent& component)
	{
		auto& model = component.m_Model;
		ImGui::Checkbox("Shadow Casting", &component.m_ShadowCasting);
		if (model) {
			ImGui::Text(model->getPath().c_str());
			if (ImGui::TreeNodeEx("SubMeshes", ImGuiTreeNodeFlags_None))
			{
				int i = 0;
				for (auto& m : model->m_Meshes) {

					std::string meshname = "Mesh" + std::to_string(i++);
					if (ImGui::TreeNodeEx(meshname.c_str(), ImGuiTreeNodeFlags_None))
					{
						auto& mat = m.GetMaterial();

						ImGui::Text(mat->name.c_str());
						ImGui::ColorEdit3("BaseColor", glm::value_ptr(mat->baseColor));
						ImGui::DragFloat("Shininess", &mat->shininess);
						ImGui::DragFloat("Metallic", &mat->metallic);
						ImGui::DragFloat("Roughness", &mat->roughness);
						if (mat->diffuseTex.m_enable) {
							auto tex = mat->diffuseTex.m_Tex->GetRendererId();

							ImGui::Text("AlbedoMap");
							ImGui::Image((void*)tex, ImVec2{ 200, 200 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
						}
						if (mat->specularTex.m_enable) {
							auto tex = mat->specularTex.m_Tex->GetRendererId();

							ImGui::Text("SpecularMap");
							ImGui::Image((void*)tex, ImVec2{ 200, 200 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
						}
						if (mat->bumpTex.m_enable) {
							auto tex = mat->bumpTex.m_Tex->GetRendererId();

							ImGui::Text("NormalMap");
							ImGui::Image((void*)tex, ImVec2{ 200, 200 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
						}


						ImGui::TreePop();
					}
				}


				ImGui::TreePop();
			}
		}
		else
			ImGui::Text("No Model Yet");



	}

	template<>
	void PropertiesPanel::DrawComponentLayout<AnimatorComponent>(AnimatorComponent& component) {
		DrawVec3Control("Pivot", component.m_Pivot);
		DrawVec3Control("Axis", component.m_Axis);
		ImGui::DragFloat("Radius", &component.m_Radius,0.1f,0.0f,10.0f);
		ImGui::DragFloat("Speed", &component.m_AngleSpeed, 0.1f, 0.0f,10.0f);
		ImGui::DragFloat("Phase", &component.m_Phase,1.0f,0.0f,360.0f);
		
	}

	template<>
	void PropertiesPanel::DrawComponentLayout<PointLightComponent>(PointLightComponent& component) {

		ImGui::ColorEdit3("Color", glm::value_ptr(component.m_Color));
		ImGui::DragFloat("Intensity", &component.m_Intensity, 0.1f, 0.0f, 10.0f);
	}
	
	template<>
	void PropertiesPanel::DrawComponentLayout<DirectionalLightComponent>(DirectionalLightComponent& component) {

		ImGui::ColorEdit3("Color", glm::value_ptr(component.m_Color));
		ImGui::DragFloat("Intensity", &component.m_Intensity, 0.1f, 0.0f, 3.0f);
		ImGui::Checkbox("Cast Shadow", &component.m_ShadowCasting);
	}

	template<>
	void PropertiesPanel::DrawComponentLayout<CameraComponent>(CameraComponent& component) {
		auto& camera = component.m_Camera;
		ImGui::Checkbox("Primary", &component.m_Primary);


		const char* projectionTypeStrings[] = { "Orthographic","Perspective" };
		const char* currentProjectionTypeString = projectionTypeStrings[(int)camera->GetType()];
		if (ImGui::BeginCombo("Projection Type", currentProjectionTypeString)) {
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
				if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
				{
					currentProjectionTypeString = projectionTypeStrings[i];
					camera->SetType((CameraType)i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if (camera->GetType() == CameraType::Perspective)
		{
			float verticalFov = glm::degrees(camera->GetPerspectiveFovy());
			if (ImGui::DragFloat("Vertical FOV", &verticalFov))
				camera->SetPerspectiveFovy(glm::radians(verticalFov));

			float Near = camera->GetPerspectiveNear();
			if (ImGui::DragFloat("Near", &Near))
				camera->SetPerspectiveNear(Near);

			float Far = camera->GetPerspectiveFar();
			if (ImGui::DragFloat("Far", &Far))
				camera->SetPerspectiveFar(Far);
		}

		if (camera->GetType() == CameraType::Orthographic)
		{
			float orthoSize = camera->GetOrthographicSize();
			if (ImGui::DragFloat("Size", &orthoSize))
				camera->SetOrthographicSize(orthoSize);

			float orthoNear = camera->GetOrthographicNear();
			if (ImGui::DragFloat("Near", &orthoNear))
				camera->SetOrthographicNear(orthoNear);

			float orthoFar = camera->GetPerspectiveNear();
			if (ImGui::DragFloat("Far", &orthoFar))
				camera->SetOrthographicFar(orthoFar);


		}
		ImGui::Checkbox("Fixed Aspect Ratio", &component.m_FixedAspectRatio);
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue , float columnWidth )
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight , lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.7f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.6f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.6f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.7f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.7f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	
}