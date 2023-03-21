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
		
		vector<int> IntDatas;
		vector<bool> BoolDatas;
		vector<float> FloatDatas;
		vector<string> StringDatas;
	};

	class BluePrint {
		friend class BluePrintNode;


	public:
		BluePrint() {};
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


		XRef<BluePrintNode> GetNodeByID(uint32_t id) {
			for (auto x : m_Nodes) {
				if (x->NodeID == id) return x;
			}
		}

		void Link(uint32_t src, uint32_t target);
		void UnLink(uint32_t src, uint32_t target);

		void GetDefaultProperties(BluePrintProperties & bp);

		template <typename T>
		T GetFieldValue(const BluePrintField& field);

		template <typename T>
		void SetFieldValue(const BluePrintField& field, T value);

		void OnRuntimeBegin();
		//传入脚本所要更新的上下文
		void OnUpdate(entt::entity e,Scene* sc, float ts,BluePrintProperties& properties);

		void Save();
		

		static XRef<BluePrint> Create(std::string m_FileName);
		uint32_t PinIDCounter = 0;

		std::vector<XRef<BluePrintField>> m_Fields;
		std::vector<XRef<BluePrintNode>> m_Nodes;

		//temp
		entt::entity m_ent;
		Scene* m_sc;
		float m_ts;
		BluePrintProperties* m_prop;
		std::vector<pair<int, int>> m_Links;
	protected:
		
		//离线。 确定节点的执行顺序，并检查不需要更新的节点
		void SortAndActiveNodes();

		bool Global = true;

		uint32_t NodeIDCounter = 0;
		
		
		


		std::string m_Name;
		std::string m_FileName;

		BluePrintProperties m_DefaultProperties;

		

		unordered_map<uint32_t, XRef<InputPin>> m_InputPins;
		unordered_map<uint32_t, XRef<OutputPin>> m_OutputPins;

	

	};

	
	

}