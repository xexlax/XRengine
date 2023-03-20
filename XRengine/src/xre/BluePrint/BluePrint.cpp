#include "BluePrint.h"
#include "xre\ECS\GameObject.h"

namespace XRE {
	BluePrint::~BluePrint()
	{
	}

	XRef<InputPin> BluePrint::MakeInput(FieldType field, std::string name)
	{
		XRef<InputPin> in = XMakeRef<InputPin>(field,name);
		in->PinID = PinIDCounter++;
		m_InputPins[in->PinID] = in;
		return in;
	}

	XRef<OutputPin> BluePrint::MakeOutput(FieldType field, std::string name)
	{
		XRef<OutputPin> out = XMakeRef<OutputPin>(field,name);
		out->PinID = PinIDCounter++;
		m_OutputPins[out->PinID] = out;
		return out;
	}

	void BluePrint::Link(uint32_t src, uint32_t target)
	{
		m_InputPins[src]->m_Connection = m_OutputPins[target].get();
		m_OutputPins[target]->m_Connections.push_back( m_InputPins[src].get());

	}

	void BluePrint::UnLink(uint32_t src, uint32_t target)
	{
		m_InputPins[src]->m_Connection =nullptr;
		auto it =
			find(m_OutputPins[target]->m_Connections.begin(),
				m_OutputPins[target]->m_Connections.end(),
				m_InputPins[src].get());
		if (it != m_OutputPins[target]->m_Connections.end())
			m_OutputPins[target]->m_Connections.erase(it);
		
	}

	void BluePrint::GetDefaultProperties(BluePrintProperties& bp)
	{
	}

	void BluePrint::OnRuntimeBegin()
	{
		SortAndActiveNodes();
	}

	void BluePrint::OnUpdate(entt::entity e, Scene* sc, float ts, BluePrintProperties& properties)
	{
		m_ent = e;
		m_sc = sc;
		m_ts = ts;
		m_prop = properties;
		for (auto x : m_Nodes) {
			if (x->m_Active == false) break;
			x->Process();
		}
	}
	void BluePrint::Save()
	{
	}
	XRef<BluePrint> BluePrint::Create(std::string m_FileName)
	{
		return XRef<BluePrint>();
	}
	void BluePrint::SortAndActiveNodes()
	{
		vector<XRef<BluePrintNode>> result,unactive;

		for (auto x : m_Nodes) {
			if (!x->NeedProcess()) {
				x->m_Active = false;
				unactive.push_back(x);
			}
			else {
				x->m_Active = true;
			}
		}
		bool findActive = true;
		while (findActive) {
			findActive = false;
			for (auto x : m_Nodes) {
				if (x->m_Active) {
					if (x->CanProcess()) {
						findActive = true;
						x->GetReady();
						if(std::find(result.begin(),result.end(),x)!= result.end())
						result.push_back(x);
					}
				}
			}
		}

		for (auto x : result) {
			x->ResetReady();
		}

		for(auto x:unactive){
			result.push_back(x);
		}

		m_Nodes = result;

		
	}

	template<>
	int BluePrint::GetFieldValue<int>(const BluePrintField& field) {
		return m_prop.IntDatas[field.id];
	}

	
	template<>
	bool BluePrint::GetFieldValue<bool>(const BluePrintField& field) {
		return m_prop.BoolDatas[field.id];
	}
	template<>
	float BluePrint::GetFieldValue<float>(const BluePrintField& field) {
		return m_prop.FloatDatas[field.id];
	}
	template<>
	std::string BluePrint::GetFieldValue<std::string>(const BluePrintField& field) {
		return m_prop.StringDatas[field.id];
	}
	

	template<>
	void BluePrint::SetFieldValue<int>(const BluePrintField& field,int v) {
		m_prop.IntDatas[field.id] = v;
	}

	template<>
	void BluePrint::SetFieldValue<bool>(const BluePrintField& field, bool v) {
		m_prop.BoolDatas[field.id] = v;
	}

	template<>
	void BluePrint::SetFieldValue<float>(const BluePrintField& field, float v) {
		m_prop.FloatDatas[field.id] = v;
	}

	template<>
	void BluePrint::SetFieldValue<std::string>(const BluePrintField& field, std::string v) {
		m_prop.StringDatas[field.id] = v;
	}
}