#pragma once

#include "BluePrint.h"
#include "BluePrintNode.h"

namespace XRE {
	class Node_Branch: public BluePrintNode{
	public:
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Bool));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
			m_Outputs[0]->m_Name = u8"Õæ";
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
			m_Outputs[1]->m_Name = u8"¼Ù";
		}

		void Process() override {
			m_Outputs[0]->ValueBool = m_Inputs[0]->GetValue<bool>();
			m_Outputs[1]->ValueBool = !m_Inputs[0]->GetValue<bool>();
		}
	};
}