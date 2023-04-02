#include "BluePrintNode.h"
#include "BluePrint.h"

namespace XRE {
	template <>
	bool OutputPin::GetValue() {
		switch (m_FieldType)
		{
		case Field_Int:
			return ValueInt == 0;
		case Field_Bool:
			return ValueBool;
		case Field_Float:
			return ValueFloat == 0;
		case Field_String:
			return ValueString == "";

		default:

			break;
		}
		return false;
	}
	template <>
	int OutputPin::GetValue() {
		switch (m_FieldType)
		{
		case Field_Int:
			return ValueInt;
		case Field_Bool:
			return ValueBool;
		case Field_Float:
			return ValueFloat;
		case Field_String:
			return std::atoi(ValueString.c_str());

		default:

			break;
		}
		return 0;
	
	}
	template <>
	float OutputPin::GetValue() {
		switch (m_FieldType)
		{
		case Field_Int:
			return ValueInt;
		case Field_Bool:
			return ValueBool;
		case Field_Float:
			return ValueFloat;
		case Field_String:
			return std::atof(ValueString.c_str());

		default:

			break;
		}
		return 0.0f;
	}
	template <>
	std::string OutputPin::GetValue() {
		switch (m_FieldType)
		{
		case Field_Int:
			return std::to_string(ValueInt);
		case Field_Bool:
			return ValueBool?"true":"false";
		case Field_Float:
			return std::to_string(ValueFloat);
		case Field_String:
			return ValueString;

		default:

			break;
		}
		return "";
		
	}

	


	bool BluePrintNode::NeedProcess()
	{
		for (auto x : m_Inputs) {
			
			if (x->m_Necessary && x->m_Connection == nullptr) return false;
		}
		for (auto x : m_Outputs) {
			
			if (x->m_Necessary && x->m_Connections.empty()) return false;
		}
		return true;
	}
	bool BluePrintNode::CanProcess()
	{
		if (m_FlowPrev != nullptr && m_FlowPrev->m_Connection != nullptr) {
			if (m_FlowPrev->m_Ready == false) return false;
		}
		for (auto x : m_Inputs) {
			if (x->m_Necessary && !x->m_Ready ) return false;
		}
		return true;
	}

	void BluePrintNode::AddInput(FieldType t)
	{
		m_Inputs.push_back(m_BluePrint->MakeInput(t));
	}

	void BluePrintNode::AddOutput(FieldType t)
	{
		m_Outputs.push_back(m_BluePrint->MakeOutput(t));
	}

	bool BluePrintNode::CanFullyProcess()
	{
		if (m_FlowPrev != nullptr && m_FlowPrev->m_Connection != nullptr) {
			if (m_FlowPrev->m_Ready == false) return false;
		}
		for (auto x : m_Inputs) {
			if (!x->m_Ready) return false;
		}
		return true;
	}
	void BluePrintNode::GetReady()
	{
		for (auto p : m_Outputs) {
			for (auto c : p->m_Connections) {
				c->m_Ready = true;
			}
		}
		if (m_FlowProc != nullptr) {
			for (auto c : m_FlowProc->m_Connections) {
				c->m_Ready = true;
			}
		}
	}
	void BluePrintNode::ResetReady()
	{
		for (auto p : m_Inputs) {
			
				p->m_Ready = false;
			
		}
		if (m_FlowPrev != nullptr) {
			m_FlowPrev->m_Ready = false;
		}
	}
	bool BluePrintNode::ControlFlowTest()
	{
		if (m_FlowPrev != nullptr) {
			if (m_FlowPrev->m_Connection != nullptr && m_FlowPrev->GetValue<bool>() == false) 
				return false;
			
		}
		return true;
	}
	void BluePrintNode::CreateControlFlow()
	{
		m_FlowPrev = m_BluePrint->MakeInput(FieldType::Field_Bool);
		m_FlowPrev->m_Necessary = false;
		m_FlowProc = m_BluePrint->MakeOutput(FieldType::Field_Bool);
		m_FlowProc->m_Necessary = false;
	}
	InputPin::~InputPin()
	{
		
			if (m_Connection != nullptr) {
				m_Connection->m_Connections.erase(
					find(
						m_Connection->m_Connections.begin(),
						m_Connection->m_Connections.end(),
						this
					)
				);
			}
		
	}
}