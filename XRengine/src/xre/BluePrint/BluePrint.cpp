#include "BluePrint.h"
#include "xre\ECS\GameObject.h"

namespace XRE {
	BluePrint::~BluePrint()
	{
	}

	void BluePrint::RemoveNode(XRef<BluePrintNode> n)
	{
		auto it = std::find(
			m_Nodes.begin(), m_Nodes.end(), n
		);
		if(it!=m_Nodes.end())
		m_Nodes.erase(
			it
		);

		//delete n.get();
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
		m_InputPins[target]->m_Connection = m_OutputPins[src].get();
		m_OutputPins[src]->m_Connections.push_back( m_InputPins[target].get());
		m_Links.emplace_back(std::pair<int, int>(src, target));

	}

	void BluePrint::UnLink(uint32_t src, uint32_t target)
	{
		m_InputPins[target]->m_Connection =nullptr;
		auto it =
			find(m_OutputPins[src]->m_Connections.begin(),	
				m_OutputPins[src]->m_Connections.end(),
				m_InputPins[target].get());
		if (it != m_OutputPins[src]->m_Connections.end())
			m_OutputPins[src]->m_Connections.erase(it);
		
		m_Links.erase(find(m_Links.begin(), m_Links.end(), std::pair<int,int>(src, target)));

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
		m_prop = &properties;
		for (auto x : m_Nodes) {
			if (x->m_Active == false) break;
			if(x->ControlFlowTest())
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
		vector<XRef<BluePrintNode>> result, unactive;
		unordered_map<XRef<BluePrintNode>, bool> visited;
	
		for (auto x : m_Nodes) {
			visited[x] = false;
			if (!x->NeedProcess()) {
				x->m_Active = false;
				unactive.push_back(x);
				visited[x] = true;
			}
			else {
				x->m_Active = true;
			}
		}
		bool findActive = true;
		while (findActive ) {
			findActive = false;
			for (auto x : m_Nodes) {
				if (x->m_Active && !visited[x] ) {
					if (x->CanProcess()) {
						findActive = true;
						x->GetReady();
						if(std::find(result.begin(),result.end(),x)== result.end())
						result.push_back(x);
						visited[x] = true;
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
		return m_prop->IntDatas[field.id];
	}

	
	template<>
	bool BluePrint::GetFieldValue<bool>(const BluePrintField& field) {
		return m_prop->BoolDatas[field.id];
	}
	template<>
	float BluePrint::GetFieldValue<float>(const BluePrintField& field) {
		return m_prop->FloatDatas[field.id];
	}
	template<>
	std::string BluePrint::GetFieldValue<std::string>(const BluePrintField& field) {
		return m_prop->StringDatas[field.id];
	}
	

	template<>
	void BluePrint::SetFieldValue<int>(const BluePrintField& field,int v) {
		m_prop->IntDatas[field.id] = v;
	}

	template<>
	void BluePrint::SetFieldValue<bool>(const BluePrintField& field, bool v) {
		m_prop->BoolDatas[field.id] = v;
	}

	template<>
	void BluePrint::SetFieldValue<float>(const BluePrintField& field, float v) {
		m_prop->FloatDatas[field.id] = v;
	}

	template<>
	void BluePrint::SetFieldValue<std::string>(const BluePrintField& field, std::string v) {
		m_prop->StringDatas[field.id] = v;
	}
}