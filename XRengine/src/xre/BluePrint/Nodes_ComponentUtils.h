
#pragma once
#include "BluePrintNode.h"
#include "BluePrint.h"

namespace XRE {

	class Node_Self :public BluePrintNode {
	public:

		Node_Self() {
			m_Title = u8"±¾¶ÔÏó";
			m_Color = Orange;

		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Int));
		}

		void Process() override {
			XRE_INFO(m_Inputs[0]->GetValue<std::string>());
		}

	};


}