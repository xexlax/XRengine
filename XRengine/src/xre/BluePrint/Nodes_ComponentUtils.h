
#pragma once
#include "BluePrintNode.h"
#include "BluePrint.h"
#include <xre\ECS\GameObject.h>

namespace XRE {

	class Node_GetByName :public BluePrintNode {
	public:

		Node_GetByName() {
			m_Title = u8"�����ֲ��Ҷ���";
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
			m_Title = u8"������";
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
			m_Title = u8"��ȡλ��";
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
			m_Title = u8"����λ��";
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
			m_Title = u8"��ȡ��ת";
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
			m_Title = u8"������ת";
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
			m_Title = u8"��ȡ�ߴ�";
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
			m_Title = u8"���óߴ�";
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
			m_Title = u8"�������ٶ�";
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
			m_Title = u8"���ý��ٶ�";
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
			m_Title = u8"��������";
			m_Color = Blue;
			m_NodeTypeID = 69;

		}
		void Initialize() override {
			AddInput(FieldType::Field_String);
			m_Inputs[0]->m_Necessary = false;
			m_Inputs[0]->m_Name = u8"ģ��";
			AddInput(FieldType::Field_String);
			m_Inputs[1]->m_Necessary = false;
			m_Inputs[1]->m_Name = u8"����";
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Int));
			CreateControlFlow();
		}

		void Process() override {
			string name = "newObj";
			if (m_Inputs[1]->m_Connection != nullptr) {
				name = m_Inputs[1]->GetValue<string>();
			}

			auto obj = m_BluePrint->m_sc->CreateGameObject(name);
			m_Outputs[0]->ValueInt = uint32_t(obj);

		}

	};

	class Node_Destroy :public BluePrintNode {
	public:

		Node_Destroy() {
			m_Title = u8"ɾ������";
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

	

}