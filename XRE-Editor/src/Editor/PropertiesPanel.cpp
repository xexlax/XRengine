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
		ImGui::Begin(u8"属性");

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

				if (ImGui::MenuItem("Rigid Body"))
				{
					if (!go.HasComponent<RigidBodyComponent>())
						go.AddComponent<RigidBodyComponent>();
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
			if (ImGui::InputText(u8"名称", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (go.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = go.GetComponent<TransformComponent>();
				DrawVec3Control(u8"位置", transform.m_Translation);
				DrawVec3Control(u8"旋转", transform.m_Rotation);
				DrawVec3Control(u8"缩放", transform.m_Scale);
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

		if (go.HasComponent<RigidBodyComponent>())
			DrawComponent<RigidBodyComponent>(go);
	}

	template<typename T>
	void PropertiesPanel::DrawComponent(GameObject go) {
		T& c = go.GetComponent<T>();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		bool open = ImGui::TreeNodeEx((void*)typeid(MeshRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, c.m_Name.c_str());
		ImGui::SameLine(ImGui::GetWindowWidth() - 120.0f);
		ImGui::Checkbox(u8"激活", &c.m_Active);
		ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
		if (ImGui::Button("+"))
		{
			ImGui::OpenPopup(u8"组件选项");
		}
		ImGui::PopStyleVar();
		bool removeComponent = false;
		if (ImGui::BeginPopup(u8"组件选项"))
		{
			if (ImGui::MenuItem(u8"删除组件"))
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
		ImGui::Checkbox(u8"阴影映射", &component.m_ShadowCasting);
		if (model) {
			ImGui::Text(model->getPath().c_str());
			if (ImGui::TreeNodeEx(u8"子网格", ImGuiTreeNodeFlags_None))
			{
				int i = 0;
				for (auto& m : model->m_Meshes) {

					std::string meshname = "Mesh" + std::to_string(i++);
					if (ImGui::TreeNodeEx(meshname.c_str(), ImGuiTreeNodeFlags_None))
					{
						auto& mat = m.GetMaterial();

						ImGui::Text(mat->name.c_str());
						ImGui::ColorEdit3(u8"基础色", glm::value_ptr(mat->baseColor));
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
			ImGui::Text(u8"未选择模型");



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

		ImGui::ColorEdit3(u8"颜色", glm::value_ptr(component.m_Color));
		ImGui::DragFloat(u8"强度", &component.m_Intensity, 0.1f, 0.0f, 10.0f);
	}
	
	template<>
	void PropertiesPanel::DrawComponentLayout<DirectionalLightComponent>(DirectionalLightComponent& component) {

		ImGui::ColorEdit3(u8"颜色", glm::value_ptr(component.m_Color));
		ImGui::DragFloat(u8"强度", &component.m_Intensity, 0.1f, 0.0f, 3.0f);
		ImGui::Checkbox(u8"阴影", &component.m_ShadowCasting);
	}

	template<>
	void PropertiesPanel::DrawComponentLayout<CameraComponent>(CameraComponent& component) {
		auto& camera = component.m_Camera;
		ImGui::Checkbox(u8"主摄像机", &component.m_Primary);


		const char* projectionTypeStrings[] = { u8"正交",u8"透视" };
		const char* currentProjectionTypeString = projectionTypeStrings[(int)camera->GetType()];
		if (ImGui::BeginCombo(u8"投影矩阵", currentProjectionTypeString)) {
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
			if (ImGui::DragFloat(u8"视域", &verticalFov))
				camera->SetPerspectiveFovy(glm::radians(verticalFov));

			float Near = camera->GetPerspectiveNear();
			if (ImGui::DragFloat(u8"近平面", &Near))
				camera->SetPerspectiveNear(Near);

			float Far = camera->GetPerspectiveFar();
			if (ImGui::DragFloat(u8"远平面", &Far))
				camera->SetPerspectiveFar(Far);
		}

		if (camera->GetType() == CameraType::Orthographic)
		{
			float orthoSize = camera->GetOrthographicSize();
			if (ImGui::DragFloat(u8"大小", &orthoSize))
				camera->SetOrthographicSize(orthoSize);

			float orthoNear = camera->GetOrthographicNear();
			if (ImGui::DragFloat(u8"近平面", &orthoNear))
				camera->SetOrthographicNear(orthoNear);

			float orthoFar = camera->GetPerspectiveNear();
			if (ImGui::DragFloat(u8"远平面", &orthoFar))
				camera->SetOrthographicFar(orthoFar);


		}
		ImGui::Checkbox(u8"固定长宽比", &component.m_FixedAspectRatio);
	}

	template<>
	void PropertiesPanel::DrawComponentLayout<RigidBodyComponent>(RigidBodyComponent& component) {

		const char* MotionTypeStrings[] = { u8"静止",u8"运动学",u8"动力学"};
		const char* ShapeTypeStrings[] = { u8"方形",u8"球形",u8"胶囊" };

		const char* currentMotionTypeString = MotionTypeStrings[component.m_MotionType];
		const char* currentShapeTypeString = ShapeTypeStrings[component.m_RigidBodyShape];
		if (ImGui::BeginCombo(u8"运动形式", currentMotionTypeString)) {
			for (int i = 0; i < 3; i++)
			{
				bool isSelected = currentMotionTypeString == MotionTypeStrings[i];
				if (ImGui::Selectable(MotionTypeStrings[i], isSelected))
				{
					currentMotionTypeString = MotionTypeStrings[i];
					component.SetMotion(i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo(u8"形状", currentShapeTypeString)) {
			for (int i = 0; i < 3; i++)
			{
				bool isSelected = currentShapeTypeString == ShapeTypeStrings[i];
				if (ImGui::Selectable(ShapeTypeStrings[i], isSelected))
				{
					currentShapeTypeString = ShapeTypeStrings[i];
					component.SetShape(i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		
		ImGui::DragFloat("Mass", &component.m_Mass, 0.1f, 0.0f, 10.0f);
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