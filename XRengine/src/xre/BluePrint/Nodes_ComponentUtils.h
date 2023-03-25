
#pragma once
#include "BluePrintNode.h"
#include "BluePrint.h"
#include <xre\ECS\GameObject.h>

namespace XRE {

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

}