#include "BluePrint.h"
#include "xre\ECS\GameObject.h"
#include "NodesDictionary.h"
#include "cereal\cereal.hpp"
#include "cereal\archives\json.hpp"
namespace XRE {
	BluePrint::~BluePrint()
	{
	}

	XRef<BluePrintNode> BluePrint::MakeNode(NodeSerializer ns)
	{
		auto x = Nodes::MakeNode(ns.NodeType);
		x->editorPosX = ns.X;
		x->editorPosY = ns.Y;
		x->NodeID = NodeIDCounter++;
		x->m_BluePrint = this;
		x->Initialize();
		m_Nodes.push_back(x);
		return x;
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

	XRef<BluePrintField> BluePrint::MakeField( FieldType t, std::string name)
	{
		XRef<BluePrintField> result = XMakeRef<BluePrintField>();
		result->m_FieldName = name;
		result->m_Type = t;
		result->id = FieldIDCounter++;
		switch (t)
		{
		case XRE::Field_Int:
			m_DefaultProperties->IntDatas[result->id]=0;
			break;
		case XRE::Field_Float:
			m_DefaultProperties->BoolDatas[result->id] =false;
			break;
		case XRE::Field_Bool:
			m_DefaultProperties->FloatDatas[result->id] = 0;
			break;
		case XRE::Field_String:
			m_DefaultProperties->StringDatas[result->id] = "";
			break;
		default:
			break;
		}

		m_Fields[result->id]=result;

		return result;
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

	XRef<BluePrintProperties> BluePrint::GetDefaultProperties()
	{
		return m_DefaultProperties;
	}

	void BluePrint::OnRuntimeBegin()
	{
		SortAndActiveNodes();
		for (auto x : m_Nodes) {
			if (x->m_Active) {
				x->Start();
			}
		}
	}

	void BluePrint::OnUpdate(entt::entity e, Scene* sc, float ts, XRef<BluePrintProperties> properties)
	{
		m_ent = e;
		m_sc = sc;
		m_ts = ts;
		m_ActiveProperties = properties;
		for (auto x : m_Nodes) {
			
			if (x->m_Active == false) break;
			if (x->ControlFlowTest())
			{
				if (x->m_FlowProc != nullptr) {
					x->m_FlowProc->ValueBool = true;
				}
				x->Process();
				
			}
			else {
				if (x->m_FlowProc != nullptr) {
					x->m_FlowProc->ValueBool = false;
				}
			}
			

			if (m_ent == entt::null && Global==false) break;
		}
	}
	void BluePrint::Save(std::string path)
	{
		if (path == "") path = m_FileName;
		m_FileName = path;
		ofstream fs(path);

		cereal::JSONOutputArchive ar(fs);
		ar(*this);

	}
	

	void BluePrint::LoadFromFile(std::string path)
	{
		m_Nodes.clear();
		m_Fields.clear();
		m_InputPins.clear();
		m_OutputPins.clear();
		m_DefaultProperties = XMakeRef<BluePrintProperties>();

		ifstream fs(path);
		m_FileName = path;
		cereal::JSONInputArchive ar(fs);
		ar(*this);
	}
	XRef<BluePrint> BluePrint::Create(std::string m_FileName)
	{
		XRef<BluePrint> r = XMakeRef<BluePrint>();

		r->LoadFromFile(m_FileName);
		return r;
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
					if (x->CanFullyProcess()) {
						findActive = true;
						x->GetReady();
						if(std::find(result.begin(),result.end(),x)== result.end())
						result.push_back(x);
						visited[x] = true;
					}
				}
			}
			if(!findActive)
			for (auto x : m_Nodes) {
				if (x->m_Active && !visited[x]) {
					if (x->CanProcess()) {
						findActive = true;
						x->GetReady();
						if (std::find(result.begin(), result.end(), x) == result.end())
							result.push_back(x);
						visited[x] = true;
					}
				}
			}
			
		}

		for (auto x : result) {
			x->ResetReady();
		}
		for (auto x : m_Nodes) {
			if (visited[x] == false)
				result.push_back(x);
		}
		for(auto x:unactive){
			result.push_back(x);
		}

		m_Nodes = result;

		
	}

	template<>
	int BluePrint::GetFieldValue<int>(XRef<BluePrintField> field) {
		return m_ActiveProperties->IntDatas[field->id];
	}

	
	template<>
	bool BluePrint::GetFieldValue<bool>(XRef<BluePrintField> field) {
		return m_ActiveProperties->BoolDatas[field->id];
	}
	template<>
	float BluePrint::GetFieldValue<float>(XRef<BluePrintField> field) {
		return m_ActiveProperties->FloatDatas[field->id];
	}
	template<>
	std::string BluePrint::GetFieldValue<std::string>(XRef<BluePrintField> field) {
		return m_ActiveProperties->StringDatas[field->id];
	}
	

	template<>
	void BluePrint::SetFieldValue<int>(XRef<BluePrintField> field,int v) {
		m_ActiveProperties->IntDatas[field->id] = v;
	}

	template<>
	void BluePrint::SetFieldValue<bool>(XRef<BluePrintField> field, bool v) {
		m_ActiveProperties->BoolDatas[field->id] = v;
	}

	template<>
	void BluePrint::SetFieldValue<float>(XRef<BluePrintField> field, float v) {
		m_ActiveProperties->FloatDatas[field->id] = v;
	}

	template<>
	void BluePrint::SetFieldValue<std::string>(XRef<BluePrintField> field, std::string v) {
		m_ActiveProperties->StringDatas[field->id] = v;
	}
	void* BluePrintProperties::GetFieldValuePointer(XRef<BluePrintField> f)
	{
		switch (f->m_Type) {
		case  FieldType::Field_Bool:
			return &BoolDatas[f->id];
		case  FieldType::Field_Int:
			return &IntDatas[f->id];
		case  FieldType::Field_Float:
			return &FloatDatas[f->id];
		case  FieldType::Field_String:
			return &StringDatas[f->id];
		}
	}
	void BluePrintProperties::Update(XRef<BluePrintProperties> src)
	{
		{
		vector<int> eraselist;
		for (auto x : IntDatas) {
			if (src->IntDatas.find(x.first) == src->IntDatas.end()) {
				eraselist.push_back(x.first);
			}
		}
		for (auto x : eraselist) {
			IntDatas.erase(x);
		}
		}
		for (auto x : src->IntDatas) {
			if (IntDatas.find(x.first) == IntDatas.end()) {
				IntDatas[x.first] = x.second;
			}
		}

		
		{
			vector<int> eraselist;
			for (auto x : BoolDatas) {
				if (src->BoolDatas.find(x.first) == src->BoolDatas.end()) {
					eraselist.push_back(x.first);
				}
			}
			for (auto x : eraselist) {
				IntDatas.erase(x);
			}
		}
		for (auto x : src->BoolDatas) {
			if (BoolDatas.find(x.first) == BoolDatas.end()) {
				BoolDatas[x.first] = x.second;
			}
		}
		{
			vector<int> eraselist;
			for (auto x : FloatDatas) {
				if (src->FloatDatas.find(x.first) == src->FloatDatas.end()) {
					eraselist.push_back(x.first);
				}
			}
			for (auto x : eraselist) {
				FloatDatas.erase(x);
			}
		}
		
		for (auto x : src->FloatDatas) {
			if (FloatDatas.find(x.first) == FloatDatas.end()) {
				FloatDatas[x.first] = x.second;
			}
		}

		{
			vector<int> eraselist;
			for (auto x : StringDatas) {
				if (src->StringDatas.find(x.first) == src->StringDatas.end()) {
					eraselist.push_back(x.first);
				}
			}
			for (auto x : eraselist) {
				StringDatas.erase(x);
			}
		}
		for (auto x : src->StringDatas) {
			if (StringDatas.find(x.first) == StringDatas.end()) {
				StringDatas[x.first] = x.second;
			}
		}
	}
}