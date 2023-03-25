#pragma once

#include "BluePrint.h"
#include "BluePrintNode.h"

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
}