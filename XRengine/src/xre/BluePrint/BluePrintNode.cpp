#include "BluePrintNode.h"


namespace XRE {
	template <>
	bool OutputPin::GetValue() {
		return ValueBool;
	}
	template <>
	int OutputPin::GetValue() {
		return ValueInt;
	}
	template <>
	float OutputPin::GetValue() {
		return ValueFloat;
	}
	template <>
	std::string OutputPin::GetValue() {
		return ValueString;
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
		for (auto x : m_Inputs) {
			if (x->m_Necessary && !x->m_Ready ) return false;
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
	}
	void BluePrintNode::ResetReady()
	{
		for (auto p : m_Inputs) {
			
				p->m_Ready = false;
			
		}
	}
}