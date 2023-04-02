#pragma once
#include "xre\Core\Input.h"

namespace XRE {
	class Node_OnKeyEvent:public BluePrintNode {
	public:
		Node_OnKeyEvent() {
			m_NodeTypeID = 81;
			m_Title = u8"键盘输入";
			m_Color = Green;
		}

		void Initialize() override {
			AddOutput(FieldType::Field_Bool);
			AddVar(Field_Int, &keycode);
			m_Variants[0].m_Name = u8"##Key";
		}

		void Process() override {
			
			m_Outputs[0]->ValueBool = Input::IsKeyPressed(keycode);
			
		}
		
		int keycode = -1;
		
	};

	class Node_OnMouseEvent: public BluePrintNode {
	public:
		Node_OnMouseEvent() {
			m_NodeTypeID = 82;
			m_Title = u8"鼠标输入";
			m_Color = Yellow;
		}

		void Initialize() override {
			AddOutput(FieldType::Field_Bool);
			m_Outputs[0]->m_Name = u8"按下";
			AddOutput(FieldType::Field_Float);
			AddOutput(FieldType::Field_Float);
			m_Outputs[1]->m_Name = u8"x";
			m_Outputs[2]->m_Name = u8"y";
			AddVar(Field_Int, &mousecode);
			m_Variants[0].m_Name = u8"##Mouse";
		}

		void Process() override {
			m_Outputs[0]->ValueBool = Input::IsMouseButtonPressed(mousecode);
		}
		int mousecode;
	};
}