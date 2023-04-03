#pragma once
#include <vector>
#include "xre\Core\Macros.h"
#include <entt/entt.hpp>
#include "BluePrintNode.h"
#include "BluePrintField.h"
#include "cereal\types\unordered_map.hpp"

#include "cereal\types\vector.hpp"
#include <cereal/archives/json.hpp>

using namespace std;
namespace XRE {
	class Scene;
	struct BluePrintProperties {
		
		unordered_map<int,int> IntDatas;
		unordered_map<int,bool> BoolDatas;
		unordered_map<int,float> FloatDatas;
		unordered_map<int,std::string> StringDatas;

		void* GetFieldValuePointer(XRef<BluePrintField> f);

		void Update(XRef<BluePrintProperties> src);

		template <class Archive>
		void serialize(Archive& ar) {
			ar(IntDatas, BoolDatas, FloatDatas, StringDatas);
		}
	};

	struct BluePrintLink {
		int inputNode, outputNode, inputOrder, outputOrder;
		BluePrintLink() {};
		BluePrintLink(int in, int on, int io, int oo) :
			inputNode(in), outputNode(on), inputOrder(io), outputOrder(oo) {

		}
		template <class Archive>
		void serialize(Archive& ar) {
			ar(inputNode, outputNode, inputOrder, outputOrder);
		}
	};

	struct NodeSerializer {
		
		int NodeType;
		float X, Y;

		NodeSerializer() = default;
		NodeSerializer(XRef<BluePrintNode> n) :
			NodeType(n->m_NodeTypeID), X(n->editorPosX), Y(n->editorPosY) {};

		template <class Archive>
		void serialize(Archive& ar) {
			ar(NodeType,X,Y);
		}
	};
	

	class BluePrint {
		friend class BluePrintNode;


	public:
		BluePrint() {
			m_DefaultProperties = XMakeRef<BluePrintProperties>();
		};
		~BluePrint();
		std::string GetName()const {
			return m_Name;
		}

		std::string GetFileName()const {
			return m_FileName;
		}

		template <typename T>
		XRef<BluePrintNode> MakeNode() {
			XRef<BluePrintNode> x = XMakeRef<T>();
			x->NodeID = NodeIDCounter++;
			x->m_BluePrint = this;
			x->Initialize();
			m_Nodes.push_back(x);
			return x;
		}

		XRef<BluePrintNode> MakeNode(NodeSerializer ns);

		void RemoveNode(XRef<BluePrintNode> n);

		XRef<InputPin> MakeInput(FieldType field, std::string name = "Input");
		XRef<OutputPin> MakeOutput(FieldType field, std::string name = "Output");

		XRef<BluePrintField> MakeField(FieldType t ,std::string name = "Var");

		XRef<BluePrintNode> GetNodeByID(uint32_t id) {
			for (auto x : m_Nodes) {
				if (x->NodeID == id) return x;
			}
		}

		void Link(uint32_t src, uint32_t target);
		void UnLink(uint32_t src, uint32_t target);

		XRef<BluePrintProperties> GetDefaultProperties();

		template <typename T>
		T GetFieldValue(XRef<BluePrintField> field);

		template <typename T>
		void SetFieldValue(XRef<BluePrintField> field, T value);

		void OnRuntimeBegin();
		//传入脚本所要更新的上下文
		void OnUpdate(entt::entity e,Scene* sc, float ts,XRef<BluePrintProperties> properties);

		void Save(std::string path = "");

		void LoadFromFile(std::string path);

		template <class Archive>
		void save(Archive& archive) const {
			archive(m_Name);

			vector<BluePrintField> fields;
			for (auto x : m_Fields) {
				fields.push_back(*(x.second));
			}
			archive(fields);
			archive(*m_DefaultProperties);


			vector<NodeSerializer> nodes;

			vector<NodeVariant> nodevariants;
			
			for (int i = 0;i < m_Nodes.size();i++) {
				auto& x = m_Nodes[i];
				int o = 0;
				for (auto ip : x->m_Inputs) {
					ip->parent = i;
					ip->order = o;
					o++;
				}
				if (x->m_FlowPrev != nullptr) {
					x->m_FlowPrev->parent = i;
					x->m_FlowPrev->order = -1;
				}
				o = 0;
				for (auto op : x->m_Outputs) {
					op->parent = i;
					op->order = o;
					o++;
				}
				if (x->m_FlowProc != nullptr) {
					x->m_FlowProc->parent = i;
					x->m_FlowProc->order = -2;
				}
				o = 0;
				for (auto v : x->m_Variants) {
					v.order = o;
					v.parent = i;
					if (v.m_Type == FieldType::Field_Field) {
						v.field_ptr = (*(XRef<BluePrintField>*)(v.m_Handle))->id;
					}
					nodevariants.push_back(v);
					o++;
					
				}
				NodeSerializer n(x);
				nodes.push_back(n);
			}
			archive(nodes);

			//SaveLink
			vector<BluePrintLink> links;
			for (auto x : m_InputPins) {
				XRef<InputPin> p = x.second;
				if (p->m_Connection != nullptr) {
					BluePrintLink l(p->parent, p->m_Connection->parent, p->order, p->m_Connection->order);
					links.push_back(l);
				}
				
			}

			archive(links);
			//SaveNodeVariant
			archive(nodevariants);
			
			

			

		}
		
		template <class Archive>
		void load(Archive& archive) {
			archive(m_Name);
			vector<NodeSerializer> nodes;

			vector<BluePrintField> fields;
			archive(fields);
			for (auto x : fields) {
				MakeField(x.m_Type, x.m_FieldName);
			}

			m_DefaultProperties = XMakeRef<BluePrintProperties>();
			archive(*m_DefaultProperties);


			
			archive(nodes);
			for (auto x : nodes) {
				MakeNode(x);
			}

			//Todo: loadLink
			vector<BluePrintLink> links;
			archive(links);
			for (auto x : links) {
				uint32_t tarid;
				if (x.inputOrder == -1)
					tarid = m_Nodes[x.inputNode]->m_FlowPrev->PinID;
				else
					tarid = m_Nodes[x.inputNode]->m_Inputs[x.inputOrder]->PinID;
				uint32_t srcid;
				if (x.outputOrder == -2)
					srcid = m_Nodes[x.outputNode]->m_FlowProc->PinID;
				else
					srcid = m_Nodes[x.outputNode]->m_Outputs[x.outputOrder]->PinID;
				Link(srcid, tarid);

			}



			vector<NodeVariant> nodevariants;

			archive(nodevariants);

			for (auto v : nodevariants) {
				m_Nodes[v.parent]->m_Variants[v.order].m_Name = v.m_Name;
				m_Nodes[v.parent]->m_Variants[v.order].m_Type = v.m_Type;
				void* h = m_Nodes[v.parent]->m_Variants[v.order].m_Handle;
				switch (v.m_Type)
				{
				case Field_Bool: {
					*(bool*)h = *(bool*)v.m_Handle;
					break;
				}

				case Field_Int: {
					*(int*)h = *(int*)v.m_Handle;
					break;
				}

				case Field_Float:
				{
					*(float*)h = *(float*)v.m_Handle;
					break;
				}
				case Field_String:
				{
					std::string s = *(string*)v.m_Handle;
					std::string& t = (*(string*)h);
					t = s;
					break;
				}
				case Field_Field:
				{
					m_Nodes[v.parent]->SetField(m_Fields[v.field_ptr]);
					
				}
				default:
					break;
				}
				
			}

			
		}

		static XRef<BluePrint> Create(std::string m_FileName);
		uint32_t PinIDCounter = 0;

		unordered_map<uint32_t,XRef<BluePrintField>>m_Fields;

		std::vector<XRef<BluePrintNode>> m_Nodes;

		//temp
		entt::entity m_ent;
		Scene* m_sc;
		float m_ts;
		XRef<BluePrintProperties> m_ActiveProperties;
		std::vector<pair<int, int>> m_Links;
		bool Global = false;
	protected:
		
		//离线。 确定节点的执行顺序，并检查不需要更新的节点
		void SortAndActiveNodes();

		

		uint32_t NodeIDCounter = 0;
		uint32_t FieldIDCounter = 0;
		
		
		


		std::string m_Name;
		std::string m_FileName;

		XRef<BluePrintProperties> m_DefaultProperties;

		

		unordered_map<uint32_t, XRef<InputPin>> m_InputPins;
		unordered_map<uint32_t, XRef<OutputPin>> m_OutputPins;

	

	};

	
	

}