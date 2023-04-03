#pragma once

#include "BluePrint.h"
#include "BluePrintNode.h"
#include <xre\ECS\GameObject.h>

namespace XRE {
	class Node_Branch: public BluePrintNode{
	public:
		Node_Branch() {
			m_NodeTypeID = 51;
			m_Title = u8"分支";
			m_Color = Yellow;
		}

		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Bool));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
			m_Outputs[0]->m_Name = u8"真";
			m_Outputs[0]->m_Necessary = false;
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
			m_Outputs[1]->m_Name = u8"假";
			m_Outputs[1]->m_Necessary = false;
		}

		void Process() override {
			m_Outputs[0]->ValueBool = m_Inputs[0]->GetValue<bool>();
			m_Outputs[1]->ValueBool = !m_Inputs[0]->GetValue<bool>();
		}
	};

	class Node_OnSceneBegin : public BluePrintNode {
	public:
		Node_OnSceneBegin() {
			m_NodeTypeID = 52;
			m_Title = u8"开始时";
			m_Color = Yellow;
		}

		void Initialize() override {
			
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
			
		}

		void Start() override {
			f = true;
		}

		void Process() override {
			m_Outputs[0]->ValueBool = f;
			f = false;
	
		}
		bool f = true;
	};

	class Node_OnCollision : public BluePrintNode {
	public:
		Node_OnCollision() {
			m_NodeTypeID = 53;
			m_Title = u8"碰撞时";
			m_Color = Yellow;
		}

		void Initialize() override {

			AddInput(Field_Int);
			m_Inputs[0]->m_Name = u8"对象1";

			AddInput(Field_Int);
			m_Inputs[1]->m_Name = u8"对象2（可选）";
			m_Inputs[1]->m_Necessary = false;

			AddOutput(Field_Bool);
			m_Outputs[0]->m_Name = u8"发生碰撞";
			m_Outputs[0]->m_Necessary = false;
			AddOutput(Field_Int);
			m_Outputs[1]->m_Name = u8"对象2";
			m_Outputs[1]->m_Necessary = false;
		}

		
		void Process() override {
			GameObject go(entt::entity(m_Inputs[0]->GetValue<int>()), m_BluePrint->m_sc);
			
			if (m_Inputs[1]->m_Connection != nullptr) {
				if (go.HasComponent<RigidBodyComponent>()) {
					auto hit = go.GetComponent<RigidBodyComponent>().m_HitObjs;
					m_Outputs[0]->ValueBool = find(hit.begin(),hit.end(),m_Inputs[1]->GetValue<int>()) != hit.end();
					m_Outputs[1]->ValueInt = m_Inputs[1]->GetValue<int>();
				}
				else {
					m_Outputs[0]->ValueBool = false;
				}
			}
			else {
				if (go.HasComponent<RigidBodyComponent>()) {
					m_Outputs[0]->ValueBool = !go.GetComponent<RigidBodyComponent>().m_HitObjs.empty();
					if(m_Outputs[0]->ValueBool == true)
					m_Outputs[1]->ValueInt = go.GetComponent<RigidBodyComponent>().m_HitObjs[0];
				}
				else {
					m_Outputs[0]->ValueBool = false;
				}
			}
			
			
			

		}

	};
}