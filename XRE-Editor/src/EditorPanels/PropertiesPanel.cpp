#include <ImGui\imgui.h>
#include "PropertiesPanel.h"
#include "PanelsManager.h"
#include "Data.h"
#include <glm\glm\gtc\type_ptr.hpp>
#include <ImGui\imgui_internal.h>

using namespace entt;
namespace XRE {
	
	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin(u8"属性");
		if (m_Switch == true) {
			ImGui::SetWindowFocus();
			m_Switch = false;
		}

		GameObject go = PanelsManager::GetScenePanel()->m_Selected;
		if (go) {
			
			DrawComponents(go);

			if (ImGui::Button(u8"添加组件"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem(u8"摄像机"))
				{
					if (!go.HasComponent<CameraComponent>())
						CommandManager::Get().Command_Create_Component<CameraComponent>(go.AddComponent<CameraComponent>(CameraType::Perspective), go);
					
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem(u8"几何渲染器"))
				{
					if (!go.HasComponent<MeshRendererComponent>())
						CommandManager::Get().Command_Create_Component<MeshRendererComponent>(go.AddComponent<MeshRendererComponent>(),go);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem(u8"点光源"))
				{
					if (!go.HasComponent<PointLightComponent>())
						CommandManager::Get().Command_Create_Component<PointLightComponent>(go.AddComponent<PointLightComponent>(),go);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem(u8"平行光"))
				{
					if (!go.HasComponent<DirectionalLightComponent>())
						CommandManager::Get().Command_Create_Component<DirectionalLightComponent>(go.AddComponent<DirectionalLightComponent>(),go);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem(u8"刚体"))
				{
					if (!go.HasComponent<RigidBodyComponent>())
						CommandManager::Get().Command_Create_Component<RigidBodyComponent>(go.AddComponent<RigidBodyComponent>(),go);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem(u8"射线"))
				{
					if (!go.HasComponent<RayComponent>())
						CommandManager::Get().Command_Create_Component<RayComponent>(go.AddComponent<RayComponent>(), go);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem(u8"蓝图脚本"))
				{
					if (!go.HasComponent<BluePrintComponent>())
						CommandManager::Get().Command_Create_Component<BluePrintComponent>(go.AddComponent<BluePrintComponent>(), go);
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

			XUI::InputText(u8"名称", &tag);
		}

		if (go.HasComponent<TransformComponent>())
		{
			auto& transform = go.GetComponent<TransformComponent>();
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				
				DrawVec3Control(u8"位置", transform.m_Translation);
				DrawVec3Control(u8"旋转", transform.m_Rotation);
				DrawVec3Control(u8"缩放", transform.m_Scale);
				/*	XUI::DragFloat3("Position", glm::value_ptr(transform.m_Translation), 0.1f);
					XUI::DragFloat3("Rotation", glm::value_ptr(transform.m_Rotation), 1.0f);
					XUI::DragFloat3("Scale", glm::value_ptr(transform.m_Scale), 0.1f);*/
				
				if(go.HasComponent<CameraComponent>()&&m_EditorCamera)
				if (ImGui::Button(u8"从编辑器摄像机衍生变换")) {
					transform.m_Translation = m_EditorCamera->GetPosition();
					transform.m_Rotation = m_EditorCamera->GetEuler();
				}
				
				
				
				ImGui::TreePop();



			}
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("x"))
			{
				ImGui::OpenPopup(u8"组件选项");
			}
			if (ImGui::BeginPopup(u8"组件选项"))
			{

				if (ImGui::MenuItem(u8"复制组件属性")) {

					m_Copied = &transform;
				}
				if (m_Copied != nullptr && m_Copied->m_Name == transform.m_Name)
					if (ImGui::MenuItem(u8"粘贴组件属性")) {

						stringstream ss;
						{
							cereal::JSONOutputArchive ar(ss);
							ar(*(TransformComponent*)(m_Copied));
						}
						{
							cereal::JSONInputArchive ar(ss);
							ar(transform);
						}

					}

				ImGui::EndPopup();
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

		if (go.HasComponent<RayComponent>())
			DrawComponent<RayComponent>(go);

		if (go.HasComponent<BluePrintComponent>())
			DrawComponent<BluePrintComponent>(go);
	}

	template<typename T>
	void PropertiesPanel::DrawComponent(GameObject go) {
		ImGui::Separator();
		T& c = go.GetComponent<T>();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, c.m_Name.c_str());
		ImGui::SameLine(ImGui::GetWindowWidth() - 120.0f);
		XUI::CheckBox(u8"激活", &c.m_Active);
		ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
		if (ImGui::Button("x"))
		{
			ImGui::OpenPopup(u8"组件选项");
		}
		ImGui::PopStyleVar();
		bool removeComponent = false;
		if (ImGui::BeginPopup(u8"组件选项"))
		{
			if (ImGui::MenuItem(u8"删除组件")) {
				CommandManager::Get().Command_Delete_Component(c, go);
				removeComponent = true;
			}

			if (ImGui::MenuItem(u8"复制组件属性")) {
				
				m_Copied = &c;
			}
			if(m_Copied!=nullptr && m_Copied->m_Name == c.m_Name)
			if (ImGui::MenuItem(u8"粘贴组件属性")) {

				stringstream ss;
				{
					cereal::JSONOutputArchive ar(ss);
					ar(*(T*)(m_Copied));
				}

				{
					cereal::JSONInputArchive ar(ss);
					ar(c);
				}

			}
				
			ImGui::EndPopup();
		}
		if (open)
		{
			DrawComponentLayout<T>(c);
			ImGui::TreePop();
		}
		if (removeComponent)
			go.RemoveComponent<T>();

	}

	template<>
	void PropertiesPanel::DrawComponentLayout<MeshRendererComponent>(MeshRendererComponent& component)
	{
		auto& model = component.m_Model;
		XUI::CheckBox(u8"阴影映射", &component.m_ShadowCasting);
		if (model) {
			ImGui::Text(model->getPath().c_str());
			if (ImGui::TreeNodeEx(u8"材质", ImGuiTreeNodeFlags_None))
			{
				if (ImGui::Button(u8"全部保存")) {
					for (auto& mat : component.m_Materials) mat->Save();
				}
				int i = 0;
				
				for (auto& mat :component.m_Materials) {
					
					
					if (mat == nullptr) {
						if (ImGui::Button(u8"空材质")) {
							ImGui::OpenPopup(u8"材质选项");
						}

						if (ImGui::BeginDragDropTarget()) {


							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetItem"))
							{
								std::string path = *(std::string*)payload->Data;

								if (path.find(".mat") != string::npos) {

									mat = ResourceManager::GetMaterial(path);

								}

							}
							ImGui::EndDragDropTarget();
						}
					}
					else
						if (ImGui::Button(mat->name.c_str())) {
							PanelsManager::GetMaterialPanel()->Select(mat,&component);
						}
						

						if (ImGui::BeginDragDropTarget()) {


							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetItem"))
							{
								std::string path = *(std::string*)payload->Data;

								if (path.find(".mat") != string::npos) {

									mat = ResourceManager::GetMaterial(path);
									
								}

							}
							ImGui::EndDragDropTarget();
						}
						
						

						if (ImGui::BeginPopup(u8"材质选项"))
						{
							if (ImGui::MenuItem(u8"新材质")) {
								XRef<Material> newMat = XMakeRef<Material>();
								newMat->name = u8"新材质";
								mat = newMat;
								newMat->Save("../Assets/materials/newMaterial.mat");
							}		

							if (ImGui::MenuItem(u8"保存")){
								mat->Save();
							}

							ImGui::EndPopup();
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
		XUI::DragFloat("Radius", &component.m_Radius,0.1f,0.0f,10.0f);
		XUI::DragFloat("Speed", &component.m_AngleSpeed, 0.1f, 0.0f,10.0f);
		XUI::DragFloat("Phase", &component.m_Phase,1.0f,0.0f,360.0f);
		
	}

	template<>
	void PropertiesPanel::DrawComponentLayout<PointLightComponent>(PointLightComponent& component) {

		ImGui::ColorEdit3(u8"颜色", glm::value_ptr(component.m_Color));
		XUI::DragFloat(u8"强度", &component.m_Intensity, 0.1f, 0.0f, 10.0f);
	}
	
	template<>
	void PropertiesPanel::DrawComponentLayout<DirectionalLightComponent>(DirectionalLightComponent& component) {

		ImGui::ColorEdit3(u8"颜色", glm::value_ptr(component.m_Color));
		XUI::DragFloat(u8"强度", &component.m_Intensity, 0.1f, 0.0f, 3.0f);
		XUI::CheckBox(u8"阴影", &component.m_ShadowCasting);
	}

	template<>
	void PropertiesPanel::DrawComponentLayout<CameraComponent>(CameraComponent& component) {
		auto& camera = component.m_Camera;


		XUI::CheckBox(u8"主摄像机", &component.m_Primary);

		
		

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
			if (XUI::DragFloat(u8"视域", &verticalFov))
				camera->SetPerspectiveFovy(glm::radians(verticalFov));

			float Near = camera->GetPerspectiveNear();
			if (XUI::DragFloat(u8"近平面", &Near))
				camera->SetPerspectiveNear(Near);

			float Far = camera->GetPerspectiveFar();
			if (XUI::DragFloat(u8"远平面", &Far))
				camera->SetPerspectiveFar(Far);
		}

		if (camera->GetType() == CameraType::Orthographic)
		{
			float orthoSize = camera->GetOrthographicSize();
			if (XUI::DragFloat(u8"大小", &orthoSize))
				camera->SetOrthographicSize(orthoSize);

			float orthoNear = camera->GetOrthographicNear();
			if (XUI::DragFloat(u8"近平面", &orthoNear))
				camera->SetOrthographicNear(orthoNear);

			float orthoFar = camera->GetPerspectiveNear();
			if (XUI::DragFloat(u8"远平面", &orthoFar))
				camera->SetOrthographicFar(orthoFar);


		}
		XUI::CheckBox(u8"固定长宽比", &component.m_FixedAspectRatio);
	}

	template<>
	void PropertiesPanel::DrawComponentLayout<RigidBodyComponent>(RigidBodyComponent& component) {

		const char* MotionTypeStrings[] = { u8"静止",u8"运动学",u8"动力学",u8"触发器"};
		const char* ShapeTypeStrings[] = { u8"方形",u8"球形",u8"胶囊" };

		const char* currentMotionTypeString = MotionTypeStrings[component.m_MotionType];
		const char* currentShapeTypeString = ShapeTypeStrings[component.m_Shape.m_Type];
		if (ImGui::BeginCombo(u8"运动形式", currentMotionTypeString)) {
			for (int i = 0; i < 4; i++)
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

		XUI::CheckBox(u8"显示碰撞体", &component.m_ShowShape);

		if (ImGui::BeginCombo(u8"形状", currentShapeTypeString)) {
			for (int i = 0; i < 3; i++)
			{
				bool isSelected = currentShapeTypeString == ShapeTypeStrings[i];
				if (ImGui::Selectable(ShapeTypeStrings[i], isSelected))
				{
					currentShapeTypeString = ShapeTypeStrings[i];
					component.m_Shape.SetType(i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if (ImGui::TreeNodeEx(u8"碰撞体形状属性", ImGuiTreeNodeFlags_None))
		{

			if (component.m_Shape.m_Type == PhysicsShape::Box) {
				DrawVec3Control(u8"半轴", component.m_Shape.m_Size);

			}
			if (component.m_Shape.m_Type == PhysicsShape::Sphere) {
				XUI::DragFloat(u8"半径", &component.m_Shape.m_Size.x);
			}
			if (component.m_Shape.m_Type == PhysicsShape::Capsule) {
				XUI::DragFloat(u8"半径", &component.m_Shape.m_Size.x, 0.1f, 0.0f, 5.0f);
				XUI::DragFloat(u8"半长轴", &component.m_Shape.m_Size.y, 0.1f, 0.0f, 5.0f);
			}

			DrawVec3Control(u8"偏移", component.m_Shape.m_Offset);

			if (component.m_Shape.m_Type != PhysicsShape::Sphere)
				DrawVec3Control(u8"旋转", component.m_Shape.m_Rotation);



			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx(u8"物理属性", ImGuiTreeNodeFlags_None))
		{
			XUI::DragFloat(u8"重力系数", &component.m_PhysicsMaterial.m_GravityFactor, 0.1f, 0.0f, 1.0f);
			XUI::DragFloat(u8"弹性系数", &component.m_PhysicsMaterial.m_Restitution, 0.1f, 0.0f, 1.0f);
			XUI::DragFloat(u8"摩擦系数", &component.m_PhysicsMaterial.m_Friction, 0.1f, 0.0f, 1.0f);
			XUI::DragFloat(u8"线速度阻尼", &component.m_PhysicsMaterial.m_LinearDampling, 0.1f, 0.0f, 1.0f);
			XUI::DragFloat(u8"角速度阻尼", &component.m_PhysicsMaterial.m_AngularDampling, 0.1f, 0.0f, 1.0f);

			ImGui::TreePop();
		}


		for (auto x : component.m_HitObjs ) {


			ImGui::Text(PanelsManager::GetScenePanel()->m_Scene->GetObj(x).GetName().c_str());
		}
		
	}

	template<>
	void PropertiesPanel::DrawComponentLayout<RayComponent>(RayComponent& component) {

		XUI::DragFloat(u8"长度", & component.m_MaxLength);
		XUI::CheckBox(u8"显示", &component.m_Display);
		ImGui::BeginChild(u8"检测结果");


		for (uint32_t x : component.m_HitObjs) {

			
			ImGui::Text(PanelsManager::GetScenePanel()->m_Scene->GetObj(x).GetName().c_str());

		}



		ImGui::EndChild();

	}

	template<>
	void PropertiesPanel::DrawComponentLayout<BluePrintComponent>(BluePrintComponent& component) {
		if (component.m_BluePrint == nullptr) {
			if (ImGui::Button(u8"New BluePrint")) {
				component.m_BluePrint = XMakeRef<BluePrint>();
				auto path = FileDialogs::SaveFile("BluePrint (*.bp)\0*.bp\0");

				if (path != "") {
					if (path.find(".bp") == string::npos) path += ".bp";
					component.m_BluePrint->Save(path);
					component.SetBP(ResourceManager::GetBluePrint(path));
				}

			}
			
		}
		else
			if (ImGui::Button(component.m_BluePrintPath.c_str())) {
				PanelsManager::GetBluePrintEditor()->m_Info = u8"对象";
				PanelsManager::GetBluePrintEditor()->m_BluePrint = component.m_BluePrint;
				PanelsManager::GetBluePrintEditor()->m_Properties = component.m_BluePrint->GetDefaultProperties();
				PanelsManager::GetBluePrintEditor()->Switch();
			}
			
		if (ImGui::BeginDragDropTarget()) {


			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetItem"))
			{
				std::string path = *(std::string*)payload->Data;

				if (path.find(".bp") != string::npos ) {
					component.SetBP(ResourceManager::GetBluePrint(path));
				}

			}
			ImGui::EndDragDropTarget();
		}
		
	
		
		if (component.m_BluePrint == nullptr) return;
		
		ImGui::BeginChild(u8"自定义属性");
		if (ImGui::Button(u8"Refresh")) {
			component.m_BluePrintProperties->Update(component.m_BluePrint->GetDefaultProperties());
		}
		
		for (auto vv : component.m_BluePrint->m_Fields) {
			ImGui::PushItemWidth(80);
			auto v = vv.second;
			//Imgui need different name to identify
			string s = "##Text" + to_string(v->id);
			string f = "##Value" + to_string(v->id);
			XUI::InputText(s.c_str(), &v->m_FieldName);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			auto& m_Properties = component.m_BluePrintProperties;
			switch (v->m_Type)
			{
			case FieldType::Field_Bool:

				XUI::CheckBox(f.c_str(), (bool*)m_Properties->GetFieldValuePointer(v));
				break;
			case FieldType::Field_Int:
				XUI::DragInt(f.c_str(), (int*)m_Properties->GetFieldValuePointer(v));
				break;
			case FieldType::Field_Float:
				XUI::DragFloat(f.c_str(), (float*)m_Properties->GetFieldValuePointer(v));
				break;
			case FieldType::Field_String:
				XUI::InputText(f.c_str(), (string*)m_Properties->GetFieldValuePointer(v));
				break;
			default:
				break;
			}

		}

		ImGui::EndChild();
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
		XUI::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.6f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.6f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		XUI::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		

		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.7f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.7f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		XUI::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	
}