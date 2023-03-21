#pragma once
#include "BluePrintNode.h"
#include "BluePrint.h"

// 00 Debug：使用Log系统输出
// 01~04 ConstInt~ConstString 常量


namespace XRE {

	class Node_Debug :public BluePrintNode {
	public: 
		
		Node_Debug() {
			m_Title = u8"调试输出";
			m_Color = Grey;
			
		}
		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_String));
			CreateControlFlow();
		}

		void Process() override {
			XRE_INFO(m_Inputs[0]->GetValue<std::string>());
		}

	};

	class Node_DeltaTime: public BluePrintNode {
	public:

		Node_DeltaTime() {
			m_Title = u8"时间差";
			m_Color = Grey;
		}

		void Initialize() override {
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
		}

		void Process() override {
			m_Outputs[0]->ValueFloat = m_BluePrint->m_ts;
		}

	};

	class Node_ConstInt : public BluePrintNode {
	public:
		Node_ConstInt() {
			m_Title = u8"整数常量";
			m_Color = Red;
		}
		void Initialize() override {
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Int));
			m_Variants.push_back(NodeVariant(Field_Int, &value));
		}

		void Process() override {
			m_Outputs[0]->ValueInt = value;
		}

		int value=0;
	};

	class Node_ConstBool : public BluePrintNode {
	public:
		Node_ConstBool() {
			m_Title = u8"布尔常量";
			m_Color = Red;
		}
		void Initialize() override {
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Bool));
			m_Variants.push_back(NodeVariant(Field_Bool, &value));
		}
		void Process() override {
			m_Outputs[0]->ValueBool = value;
		}

		bool value=false;
	};

	class Node_ConstFloat : public BluePrintNode {
	public:
		
		Node_ConstFloat() {
			m_Title = u8"浮点常量";
			m_Color = Red;
		}

		void Initialize() override {
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Float));
			m_Variants.push_back(NodeVariant(Field_Float, &value));
		}

		void Process() override {
			m_Outputs[0]->ValueFloat = value;
		}

		float value=0;
	};


	class Node_ConstString : public BluePrintNode {
	public:
		Node_ConstString() {
			m_Title = u8"字符串常量";
			m_Color = Red;
		}
		void Initialize() override {
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_String));
			m_Variants.push_back(NodeVariant(Field_String, &value));
		}

		void Process() override {
			m_Outputs[0]->ValueString = value;
		}

		std::string value="";
	};


	class Node_GetField : public BluePrintNode {
	public:
		Node_GetField() {
			m_Title = u8"获取变量";
			m_Color = Purple;
		}

		void Initialize() override {
			m_Outputs.push_back(m_BluePrint->MakeOutput(FieldType::Field_Blank));
			m_Variants.push_back(NodeVariant(Field_Field, &m_field));
			CreateControlFlow();
		}

		void SetField(BluePrintField field) {
			m_field = field;
			m_Outputs[0]->m_FieldType = field.m_Type;
		}

		void Process() override {
			
			switch (m_Outputs[0]->m_FieldType)
			{
			case Field_Int:
				m_Outputs[0]->ValueInt = m_BluePrint->GetFieldValue<int>(m_field);
				break;
			case Field_Bool:
				m_Outputs[0]->ValueBool = m_BluePrint->GetFieldValue<bool>(m_field);
				break;
			case Field_Float:
				m_Outputs[0]->ValueFloat = m_BluePrint->GetFieldValue<float>(m_field);
				break;
			case Field_String:
				m_Outputs[0]->ValueString = m_BluePrint->GetFieldValue<string>(m_field);
				break;
			default:
				break;
			}
		}

		BluePrintField m_field;
		
	};

	class Node_SetField : public BluePrintNode {
	public:
		Node_SetField(){
			m_Title = u8"设置变量";
			m_Color = Purple;
			
		}

		void Initialize() override {
			m_Inputs.push_back(m_BluePrint->MakeInput(FieldType::Field_Blank));
			m_Variants.push_back(NodeVariant(Field_Field, &m_field));
			CreateControlFlow();
		}

		void SetField(BluePrintField field) {
			m_field = field;
			m_Inputs[0]->m_FieldType = field.m_Type;
		}

		void Process() override {

			switch (m_Outputs[0]->m_FieldType)
			{
			case Field_Int:
				ivalue = m_Inputs[0]->GetValue<int>();
				m_BluePrint->SetFieldValue<int>(m_field,ivalue);
				break;
			case Field_Bool:
				bvalue = m_Inputs[0]->GetValue<bool>();
				m_BluePrint->SetFieldValue<bool>(m_field,bvalue);
				break;
			case Field_Float:
				fvalue = m_Inputs[0]->GetValue<float>();
				m_BluePrint->SetFieldValue<float>(m_field,fvalue);
				break;
			case Field_String:
				svalue = m_Inputs[0]->GetValue<std::string>();
				m_BluePrint->SetFieldValue<string>(m_field,svalue);
				break;
			default:
				break;
			}
		}

		BluePrintField m_field;
		int ivalue;
		bool bvalue;
		float fvalue;
		std::string svalue;
	};
}