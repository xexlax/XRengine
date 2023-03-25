
#pragma once
#include "BluePrintNode.h"
#include "BluePrint.h"
#include <glm\trigonometric.hpp>

namespace XRE {

	class Node_Plus : public BluePrintNode {
	public:
		Node_Plus() {
			m_Title = u8"+";
			m_Color = Red;
			m_NodeTypeID = 21;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
		}

		void Process() override {
			m_Outputs[0]->ValueFloat = m_Inputs[0]->GetValue<float>() + m_Inputs[1]->GetValue<float>();
		}
	};

	class Node_Minus : public BluePrintNode {
	public:
		Node_Minus() {
			m_Title = u8"-";
			m_Color = Red;
			m_NodeTypeID = 22;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
		}

		void Process() override {
			m_Outputs[0]->ValueFloat = m_Inputs[0]->GetValue<float>() - m_Inputs[1]->GetValue<float>();
		}
	};

	class Node_Multiply : public BluePrintNode {
	public:
		Node_Multiply() {
			m_Title = u8"*";
			m_Color = Red;
			m_NodeTypeID = 23;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
		}

		void Process() override {
			m_Outputs[0]->ValueFloat = m_Inputs[0]->GetValue<float>() * m_Inputs[1]->GetValue<float>();
		}
	};

	class Node_Division : public BluePrintNode {
	public:
		Node_Division() {
			m_Title = u8"/";
			m_Color = Red;
			m_NodeTypeID = 24;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
		}

		void Process() override {
			m_Outputs[0]->ValueFloat = m_Inputs[0]->GetValue<float>() / m_Inputs[1]->GetValue<float>();
		}
	};

	class Node_Mod : public BluePrintNode {
	public:
		Node_Mod(){
			m_Title = u8"Mod";
			m_Color = Red;
			m_NodeTypeID = 25;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Int));
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Int));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Int));
		}

		void Process() override {
			int a = m_Inputs[0]->GetValue<int>();
			int b= m_Inputs[1]->GetValue<int>();
			m_Outputs[0]->ValueInt = a - (a / b) * b;
		}
	};

	class Node_Sin : public BluePrintNode {
	public:
		Node_Sin() {
			m_Title = u8"sin";
			m_Color = Red;
			m_NodeTypeID = 26;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
		}

		void Process() override {
			float a = m_Inputs[0]->GetValue<float>();

			m_Outputs[0]->ValueFloat = sin(a);
		}
	};

	class Node_Cos: public BluePrintNode {
	public:
		Node_Cos() {
			m_Title = u8"cos";
			m_Color = Red;
			m_NodeTypeID = 27;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
		}

		void Process() override {
			float a = m_Inputs[0]->GetValue<float>();

			m_Outputs[0]->ValueFloat = cos(a);
		}
	};

	class Node_Tan : public BluePrintNode {
	public:
		Node_Tan() {
			m_Title = u8"tan";
			m_Color = Red;
			m_NodeTypeID = 28;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
		}

		void Process() override {
			float a = m_Inputs[0]->GetValue<float>();

			m_Outputs[0]->ValueFloat = tan(a);
		}
	};

	class Node_RadToDegree : public BluePrintNode {
	public:
		Node_RadToDegree(){
			m_Title = u8"RadToDegree";
			m_Color = Red;
			m_NodeTypeID = 29;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
		}

		void Process() override {
			float a = m_Inputs[0]->GetValue<float>();

			m_Outputs[0]->ValueFloat = glm::degrees(a);
		}
	};

	class Node_DegreeToRad : public BluePrintNode {
	public:
		Node_DegreeToRad() {
			m_Title = u8"DegreeToRad";
			m_Color = Red;
			m_NodeTypeID = 30;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
		}

		void Process() override {
			float a = m_Inputs[0]->GetValue<float>();

			m_Outputs[0]->ValueFloat = glm::radians(a);
		}
	};

	class Node_And : public BluePrintNode {
	public:
		Node_And() {
			m_Title = u8"and";
			m_Color = Red;
			m_NodeTypeID = 41;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Bool));
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Bool));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
		}

		void Process() override {
			m_Outputs[0]->ValueBool = m_Inputs[0]->GetValue<bool>() && m_Inputs[1]->GetValue<bool>();
		}
	};

	class Node_Or : public BluePrintNode {
	public:
		Node_Or() {
			m_Title = u8"or";
			m_Color = Red;
			m_NodeTypeID = 42;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Bool));
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Bool));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
		}

		void Process() override {
			m_Outputs[0]->ValueBool = m_Inputs[0]->GetValue<bool>()|| m_Inputs[1]->GetValue<bool>();
		}
	};

	class Node_Not : public BluePrintNode {
	public:
		Node_Not() {
			m_Title = u8"not";
			m_Color = Red;
			m_NodeTypeID = 43;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Bool));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
		}

		void Process() override {
			m_Outputs[0]->ValueBool =! m_Inputs[0]->GetValue<bool>();
		}
	};

	class Node_Equal : public BluePrintNode {
	public:
		Node_Equal() {
			m_Title = u8"==";
			m_Color = Red;
			m_NodeTypeID = 44;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
		}

		void Process() override {
			m_Outputs[0]->ValueBool = m_Inputs[0]->GetValue<float>() == m_Inputs[1]->GetValue<float>();
		}
	};

	class Node_Compare : public BluePrintNode {
	public:
		Node_Compare() {
			m_Title = u8">";
			m_Color = Red;
			m_NodeTypeID = 45;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
		}

		void Process() override {
			m_Outputs[0]->ValueBool = m_Inputs[0]->GetValue<float>() > m_Inputs[1]->GetValue<float>();
		}
	};

	class Node_EqualOrCompare : public BluePrintNode {
	public:
		Node_EqualOrCompare() {
			m_Title = u8">=";
			m_Color = Red;
			m_NodeTypeID = 46;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
		}

		void Process() override {
			m_Outputs[0]->ValueBool = m_Inputs[0]->GetValue<float>() >= m_Inputs[1]->GetValue<float>();
		}
	};


	class Node_UnEqual : public BluePrintNode {
	public:
		Node_UnEqual() {
			m_Title = u8"!=";
			m_Color = Red;
			m_NodeTypeID = 47;
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Float));
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
		}

		void Process() override {
			m_Outputs[0]->ValueBool = m_Inputs[0]->GetValue<float>() != m_Inputs[1]->GetValue<float>();
		}
	};


}