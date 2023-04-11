
#pragma once
#include "BluePrintNode.h"
#include "BluePrint.h"
#include <xre\ECS\GameObject.h>

namespace XRE {

	class Node_GetByName :public BluePrintNode {
	public:

		Node_GetByName() {
			m_Title = u8"按名字查找对象";
			m_Color = Blue;
			m_NodeTypeID = 59;

		}
		void Initialize() override {
			AddInput(FieldType::Field_String);
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Int));
		}

		void Process() override {

			auto objs = m_BluePrint->m_sc->FindGOByName(m_Inputs[0]->GetValue<string>());
			if (objs.size() > 0)
				m_Outputs[0]->ValueInt = uint32_t(objs[0]);
			else
				m_Outputs[0]->ValueInt = -2;
		}

	};
	

	class Node_Self :public BluePrintNode {
	public:

		Node_Self() {
			m_Title = u8"本对象";
			m_Color = Blue;
			m_NodeTypeID = 60;

		}
		void Initialize() override {
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Int));
		}

		void Process() override {

			m_Outputs[0]->ValueInt = uint32_t(m_BluePrint->m_ent) ;
			
		}

	};

	class Node_GetPosition :public BluePrintNode {
	public:

		Node_GetPosition(){
			m_Title = u8"获取位置";
			m_Color = Blue;
			m_NodeTypeID = 61;

		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Int));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
			m_Outputs[0]->m_Necessary = false;
			m_Outputs[0]->m_Name = "x";
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
			m_Outputs[1]->m_Necessary = false;
			m_Outputs[1]->m_Name = "y";
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
			m_Outputs[2]->m_Necessary = false;
			m_Outputs[2]->m_Name = "z";

		}

		void Process() override {
			GameObject go(entt::entity(m_Inputs[0]->GetValue<int>()), m_BluePrint->m_sc);
			glm::vec3 pos = go.GetComponent<TransformComponent>().GetGlobalTranslation();
			m_Outputs[0]->ValueFloat = pos.x;
			m_Outputs[1]->ValueFloat = pos.y;
			m_Outputs[2]->ValueFloat = pos.z;
		}

	};

	class Node_SetPosition :public BluePrintNode {
	public:

		Node_SetPosition() {
			m_Title = u8"设置位置";
			m_Color = Blue;
			m_NodeTypeID = 62;

		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Int));

			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[1]->m_Necessary = false;
			m_Inputs[1]->m_Name = "x";
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[2]->m_Necessary = false;
			m_Inputs[2]->m_Name = "y";
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[3]->m_Necessary = false;
			m_Inputs[3]->m_Name = "z";
		}

		void Process() override {
			GameObject go(entt::entity(m_Inputs[0]->GetValue<int>()), m_BluePrint->m_sc);
			
			go.GetComponent<TransformComponent>().SetGlobalTranslation(
				glm::vec3(
					m_Inputs[1]->GetValue<float>(),
					m_Inputs[2]->GetValue<float>(),
					m_Inputs[3]->GetValue<float>()
				)
			);
		   

		}

	};

	class Node_GetRotation :public BluePrintNode {
	public:

		Node_GetRotation() {
			m_Title = u8"获取旋转";
			m_Color = Blue;
			m_NodeTypeID = 63;

		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Int));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
			m_Outputs[0]->m_Necessary = false;
			m_Outputs[0]->m_Name = "x";
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
			m_Outputs[1]->m_Necessary = false;
			m_Outputs[1]->m_Name = "y";
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
			m_Outputs[2]->m_Necessary = false;
			m_Outputs[2]->m_Name = "z";

		}

		void Process() override {
			GameObject go(entt::entity(m_Inputs[0]->GetValue<int>()), m_BluePrint->m_sc);
			glm::vec3 pos = go.GetComponent<TransformComponent>().GetGlobalEuler();
			m_Outputs[0]->ValueFloat = pos.x;
			m_Outputs[1]->ValueFloat = pos.y;
			m_Outputs[2]->ValueFloat = pos.z;
		}

	};

	class Node_SetRotation :public BluePrintNode {
	public:

		Node_SetRotation() {
			m_Title = u8"设置旋转";
			m_Color = Blue;
			m_NodeTypeID = 64;

		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Int));

			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[1]->m_Necessary = false;
			m_Inputs[1]->m_Name = "x";
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[2]->m_Necessary = false;
			m_Inputs[2]->m_Name = "y";
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[3]->m_Necessary = false;
			m_Inputs[3]->m_Name = "z";
		}

		void Process() override {
			GameObject go(entt::entity(m_Inputs[0]->GetValue<int>()), m_BluePrint->m_sc);

			go.GetComponent<TransformComponent>().SetGlobalRotation(
				glm::vec3(
					m_Inputs[1]->GetValue<float>(),
					m_Inputs[2]->GetValue<float>(),
					m_Inputs[3]->GetValue<float>()
				)
			);


		}

	};

	class Node_GetScale :public BluePrintNode {
	public:

		Node_GetScale() {
			m_Title = u8"获取尺寸";
			m_Color = Blue;
			m_NodeTypeID = 65;

		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Int));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
			m_Outputs[0]->m_Necessary = false;
			m_Outputs[0]->m_Name = "x";
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
			m_Outputs[1]->m_Necessary = false;
			m_Outputs[1]->m_Name = "y";
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
			m_Outputs[2]->m_Necessary = false;
			m_Outputs[2]->m_Name = "z";

		}

		void Process() override {
			GameObject go(entt::entity(m_Inputs[0]->GetValue<int>()), m_BluePrint->m_sc);
			glm::vec3 pos = go.GetComponent<TransformComponent>().GetGlobalScale();
			m_Outputs[0]->ValueFloat = pos.x;
			m_Outputs[1]->ValueFloat = pos.y;
			m_Outputs[2]->ValueFloat = pos.z;
		}

	};

	class Node_SetScale :public BluePrintNode {
	public:

		Node_SetScale() {
			m_Title = u8"设置尺寸";
			m_Color = Blue;
			m_NodeTypeID = 66;

		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Int));

			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[1]->m_Necessary = false;
			m_Inputs[1]->m_Name = "x";
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[2]->m_Necessary = false;
			m_Inputs[2]->m_Name = "y";
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[3]->m_Necessary = false;
			m_Inputs[3]->m_Name = "z";
		}

		void Process() override {
			GameObject go(entt::entity(m_Inputs[0]->GetValue<int>()), m_BluePrint->m_sc);

			go.GetComponent<TransformComponent>().SetGlobalScale(
				glm::vec3(
					m_Inputs[1]->GetValue<float>(),
					m_Inputs[2]->GetValue<float>(),
					m_Inputs[3]->GetValue<float>()
				)
			);


		}

	};

	class Node_SetLinearVelocity :public BluePrintNode {
	public:

		Node_SetLinearVelocity() {
			m_Title = u8"设置线速度";
			m_Color = Blue;
			m_NodeTypeID = 67;

		}
		void Initialize() override {
			CreateControlFlow();
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Int));

			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[1]->m_Necessary = false;
			m_Inputs[1]->m_Name = "x";
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[2]->m_Necessary = false;
			m_Inputs[2]->m_Name = "y";
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[3]->m_Necessary = false;
			m_Inputs[3]->m_Name = "z";
		}

		void Process() override {
			GameObject go(entt::entity(m_Inputs[0]->GetValue<int>()), m_BluePrint->m_sc);
			if (go.HasComponent<RigidBodyComponent>()) {
				go.GetComponent<RigidBodyComponent>().m_Motion.linear_velocity =
					glm::vec3(
						m_Inputs[1]->GetValue<float>(),
						m_Inputs[2]->GetValue<float>(),
						m_Inputs[3]->GetValue<float>()
					);
				go.GetComponent<RigidBodyComponent>().m_Motion.needUpdate = true;
			}
				


		}

	};

	class Node_SetAngularVelocity :public BluePrintNode {
	public:

		Node_SetAngularVelocity() {
			m_Title = u8"设置角速度";
			m_Color = Blue;
			m_NodeTypeID = 68;

		}
		void Initialize() override {
			CreateControlFlow();
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Int));

			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[1]->m_Necessary = false;
			m_Inputs[1]->m_Name = "x";
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[2]->m_Necessary = false;
			m_Inputs[2]->m_Name = "y";
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs[3]->m_Necessary = false;
			m_Inputs[3]->m_Name = "z";
		}

		void Process() override {
			GameObject go(entt::entity(m_Inputs[0]->GetValue<int>()), m_BluePrint->m_sc);
			if (go.HasComponent<RigidBodyComponent>()) {
				go.GetComponent<RigidBodyComponent>().m_Motion.angular_velocity =
					glm::vec3(
						m_Inputs[1]->GetValue<float>(),
						m_Inputs[2]->GetValue<float>(),
						m_Inputs[3]->GetValue<float>()
					);
				go.GetComponent<RigidBodyComponent>().m_Motion.needUpdate = true;
			}
		}

	};

	class Node_Create :public BluePrintNode {
	public:

		Node_Create() {
			m_Title = u8"创建对象";
			m_Color = Blue;
			m_NodeTypeID = 69;

		}
		void Initialize() override {
			AddInput(FieldType::Field_String);
			m_Inputs[0]->m_Necessary = false;
			m_Inputs[0]->m_Name = u8"模板";
			AddInput(FieldType::Field_String);
			m_Inputs[1]->m_Necessary = false;
			m_Inputs[1]->m_Name = u8"名称";
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Int));
			CreateControlFlow();
		}

		void Process() override {
			string name = "newObj";
			if (m_Inputs[1]->m_Connection != nullptr) {
				name = m_Inputs[1]->GetValue<string>();
			}

			if (m_Inputs[0]->m_Connection != nullptr) {
				auto obj = m_BluePrint->m_sc->CreateFromFile(ResourceManager::GetFullPath(m_Inputs[0]->GetValue<string>()));
				if (obj.HasComponent<RigidBodyComponent>()) {
					auto& rbc = obj.GetComponent<RigidBodyComponent>();
					auto& tc = obj.GetComponent<TransformComponent>();
					if (rbc.m_Active && rbc.m_MotionType != RigidBodyComponent::RigidBodyMotion::Trigger) {

						rbc.m_PhysicObj = m_BluePrint->m_sc->m_PhysicsScene->CreateRigidBody(tc, rbc);
						rbc.m_PhysicsMaterial.changed = false;
					}
				}
				m_Outputs[0]->ValueInt = uint32_t(obj);
			}
			else {
				auto obj = m_BluePrint->m_sc->CreateGameObject(name);
				m_Outputs[0]->ValueInt = uint32_t(obj);
			}

		}

	};

	class Node_Destroy :public BluePrintNode {
	public:

		Node_Destroy() {
			m_Title = u8"删除对象";
			m_Color = Blue;
			m_NodeTypeID = 70;

		}
		void Initialize() override {
			AddInput(FieldType::Field_Int);
			CreateControlFlow();
			
		}

		void Process() override {
			if (m_Inputs[0]->GetValue<int>()>=0 && m_BluePrint->m_sc->GetObj(m_Inputs[0]->GetValue<int>()) ) {
				auto ent = entt::entity(m_Inputs[0]->GetValue<int>());
				GameObject go(ent, m_BluePrint->m_sc);
				if (ent == m_BluePrint->m_ent) m_BluePrint->m_ent = entt::null;
				m_BluePrint->m_sc->Destroy(go);
			}
			
		}

	};


	class Node_GetBluePrintField :public BluePrintNode {
	public:

		
		int m_VarTypeAndIdx;

		Node_GetBluePrintField() {
			m_Title = u8"获取外部变量";
			m_Color = Blue;
			m_NodeTypeID = 71;

		}
		void Initialize() override {
			AddInput(FieldType::Field_Int);
			m_Inputs[0]->m_Name = u8"对象";

			AddOutput(FieldType::Field_Blank);
			AddVar(FieldType::Field_Int, &m_VarTypeAndIdx);
			m_Variants[0].m_Name = u8"##ExternVar";
			CreateControlFlow();

		}

		void Process() override {
			GameObject go(entt::entity(m_Inputs[0]->GetValue<int>()), m_BluePrint->m_sc);
			if (go.HasComponent<BluePrintComponent>()) {
				auto bp = go.GetComponent<BluePrintComponent>();
				FieldType t =FieldType( m_VarTypeAndIdx / 100);
				int idx = m_VarTypeAndIdx - t * 100;
				switch (t)
				{
				case XRE::Field_Int:
					m_Outputs[0]->m_FieldType = FieldType::Field_Int;
					m_Outputs[0]->ValueInt = bp.m_BluePrintProperties->IntDatas[idx];
					break;
				case XRE::Field_Float:
					m_Outputs[0]->m_FieldType = FieldType::Field_Float;
					m_Outputs[0]->ValueFloat = bp.m_BluePrintProperties->FloatDatas[idx];
					break;
				case XRE::Field_Bool:
					m_Outputs[0]->m_FieldType = FieldType::Field_Bool;
					m_Outputs[0]->ValueBool = bp.m_BluePrintProperties->BoolDatas[idx];
					break;
				case XRE::Field_String:
					m_Outputs[0]->m_FieldType = FieldType::Field_String;
					m_Outputs[0]->ValueString = bp.m_BluePrintProperties->StringDatas[idx];
					break;
				
				default:
					break;
				}

			}

		}
		


	};

	class Node_SetBluePrintField :public BluePrintNode {
	public:


		int m_VarTypeAndIdx;

		Node_SetBluePrintField() {
			m_Title = u8"设置外部变量";
			m_Color = Blue;
			m_NodeTypeID = 72;

		}
		void Initialize() override {
			AddInput(FieldType::Field_Int);
			m_Inputs[0]->m_Name = u8"对象";

			AddInput(FieldType::Field_Blank);
			AddVar(FieldType::Field_Int, &m_VarTypeAndIdx);
			m_Variants[0].m_Name = u8"##ExternVar";
			CreateControlFlow();

		}

		void Process() override {

			GameObject go(entt::entity(m_Inputs[0]->GetValue<int>()), m_BluePrint->m_sc);
			if (go.HasComponent<BluePrintComponent>()) {
				auto bp = go.GetComponent<BluePrintComponent>();
				FieldType t = FieldType(m_VarTypeAndIdx / 100);
				int idx = m_VarTypeAndIdx - t * 100;
				switch (t)
				{
				case XRE::Field_Int:
					m_Inputs[1]->m_FieldType = FieldType::Field_Int;
					bp.m_BluePrintProperties->IntDatas[idx] = m_Inputs[1]->GetValue<int>();
					break;
				case XRE::Field_Float:
					m_Inputs[1]->m_FieldType = FieldType::Field_Float;
					bp.m_BluePrintProperties->FloatDatas[idx] = m_Inputs[1]->GetValue<float>();
					break;
				case XRE::Field_Bool:
					m_Inputs[1]->m_FieldType = FieldType::Field_Bool;
					bp.m_BluePrintProperties->BoolDatas[idx] = m_Inputs[1]->GetValue<bool>();
					break;
				case XRE::Field_String:
					m_Inputs[1]->m_FieldType = FieldType::Field_String;
					bp.m_BluePrintProperties->StringDatas[idx] = m_Inputs[1]->GetValue<string>();
					break;

				default:
					break;
				}

			}

		}


	};

	
	class Node_SetMaterial :public BluePrintNode {
	public:
		string m_Mat;

		Node_SetMaterial() {
			m_Title = u8"设置材质";
			m_Color = Blue;
			m_NodeTypeID = 73;

		}
		void Initialize() override {
			AddInput(FieldType::Field_Int);
			m_Inputs[0]->m_Name = u8"对象";

			AddVar(FieldType::Field_String, &m_Mat);
			m_Variants[0].m_Name = u8"材质";
			CreateControlFlow();

		}

		void Process() override {
			if (m_Inputs[0]->GetValue<int>() >= 0 && m_BluePrint->m_sc->GetObj(m_Inputs[0]->GetValue<int>())) {
				auto ent = entt::entity(m_Inputs[0]->GetValue<int>());
				GameObject go(ent, m_BluePrint->m_sc);
				go.GetComponent<MeshRendererComponent>().m_Materials[0] = ResourceManager::GetMaterial(m_Mat);
			}
		}
	};


	class Node_SetRigidBodyMotion:public BluePrintNode {
	public:
		int rbStatus;

		Node_SetRigidBodyMotion() {
			m_Title = u8"设置刚体状态";
			m_Color = Blue;
			m_NodeTypeID = 74;

		}
		void Initialize() override {
			AddInput(FieldType::Field_Int);
			m_Inputs[0]->m_Name = u8"对象";

			
			AddVar(FieldType::Field_Int, &rbStatus);
			m_Variants[0].m_Name = u8"运动形式";
			CreateControlFlow();

		}

		void Process() override {
			if (m_Inputs[0]->GetValue<int>() >= 0 && m_BluePrint->m_sc->GetObj(m_Inputs[0]->GetValue<int>())) {
				auto ent = entt::entity(m_Inputs[0]->GetValue<int>());
				GameObject go(ent, m_BluePrint->m_sc);
				go.GetComponent<RigidBodyComponent>().m_MotionType = RigidBodyComponent::RigidBodyMotion(rbStatus);
			}
		}
	};


	class Node_SetPointLight :public BluePrintNode {
	public:
		int rbStatus;

		Node_SetPointLight(){
			m_Title = u8"设置点光源";
			m_Color = Blue;
			m_NodeTypeID = 75;

		}
		void Initialize() override {
			AddInput(FieldType::Field_Int);
			m_Inputs[0]->m_Name = u8"对象";

			AddInput(FieldType::Field_Float);
			m_Inputs[1]->m_Name = u8"强度";
			m_Inputs[1]->m_Necessary = false;

			AddInput(FieldType::Field_Float);
			m_Inputs[2]->m_Name = u8"r";
			m_Inputs[2]->m_Necessary = false;
			AddInput(FieldType::Field_Float);
			m_Inputs[3]->m_Name = u8"g";
			m_Inputs[3]->m_Necessary = false;
			AddInput(FieldType::Field_Float);
			m_Inputs[4]->m_Name = u8"b";
			m_Inputs[4]->m_Necessary = false;
			

			CreateControlFlow();

		}

		void Process() override {
			if (m_Inputs[0]->GetValue<int>() >= 0 && m_BluePrint->m_sc->GetObj(m_Inputs[0]->GetValue<int>())) {
				auto ent = entt::entity(m_Inputs[0]->GetValue<int>());
				GameObject go(ent, m_BluePrint->m_sc);
				if(m_Inputs[1]->m_Connection)
				go.GetComponent<PointLightComponent>().m_Intensity = m_Inputs[1]->GetValue<float>();

				if (m_Inputs[2]->m_Connection)
					go.GetComponent<PointLightComponent>().m_Color.r = m_Inputs[2]->GetValue<float>();
				if (m_Inputs[3]->m_Connection)
					go.GetComponent<PointLightComponent>().m_Color.r = m_Inputs[3]->GetValue<float>();
				if (m_Inputs[4]->m_Connection)
					go.GetComponent<PointLightComponent>().m_Color.r = m_Inputs[4]->GetValue<float>();
			}
		}
	};




}