#pragma once
#include <vector>
#include "xre\Core\Macros.h"
#include <entt/entt.hpp>
#include "BluePrintNode.h"
#include "BluePrintField.h"

using namespace std;
namespace XRE {
	class Scene;
	struct BluePrintProperties {
		
		unordered_map<int,int> IntDatas;
		unordered_map<int,bool> BoolDatas;
		unordered_map<int,float> FloatDatas;
		unordered_map<int,string> StringDatas;

		void* GetFieldValuePointer(XRef<BluePrintField> f);

		void Update(XRef<BluePrintProperties> src);
	};

	class BluePrint {
		friend class BluePrintNode;


	public:
		BluePrint() {
			m_DefaultProperties = XMakeRef<BluePrintProperties>();
		};
		~BluePrint();

		template <typename T>
		XRef<BluePrintNode> MakeNode() {
			XRef<BluePrintNode> x = XMakeRef<T>();
			x->NodeID = NodeIDCounter++;
			x->m_BluePrint = this;
			x->Initialize();
			m_Nodes.push_back(x);
			return x;
		}

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
		//����ű���Ҫ���µ�������
		void OnUpdate(entt::entity e,Scene* sc, float ts,XRef<BluePrintProperties> properties);

		void Save();
		

		static XRef<BluePrint> Create(std::string m_FileName);
		uint32_t PinIDCounter = 0;

		std::vector<XRef<BluePrintField>> m_Fields;

		std::vector<XRef<BluePrintNode>> m_Nodes;

		//temp
		entt::entity m_ent;
		Scene* m_sc;
		float m_ts;
		XRef<BluePrintProperties> m_ActiveProperties;
		std::vector<pair<int, int>> m_Links;
	protected:
		
		//���ߡ� ȷ���ڵ��ִ��˳�򣬲���鲻��Ҫ���µĽڵ�
		void SortAndActiveNodes();

		bool Global = true;

		uint32_t NodeIDCounter = 0;
		uint32_t FieldIDCounter = 0;
		
		
		


		std::string m_Name;
		std::string m_FileName;

		XRef<BluePrintProperties> m_DefaultProperties;

		

		unordered_map<uint32_t, XRef<InputPin>> m_InputPins;
		unordered_map<uint32_t, XRef<OutputPin>> m_OutputPins;

	

	};

	
	

}