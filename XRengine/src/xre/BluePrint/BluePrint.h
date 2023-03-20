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


	public:
		BluePrint() = default;
		~BluePrint();

		template <typename T>
		XRef<BluePrintNode> MakeNode() {
			XRef<BluePrintNode> x = XMakeRef<T>();
			x->NodeID = NodeIDCounter++;
			m_Nodes.push_back(x);
		}

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
		//����ű���Ҫ���µ�������
		void OnUpdate(entt::entity e,Scene* sc, float ts,BluePrintProperties& properties);

		void Save();
		

		static XRef<BluePrint> Create(std::string m_FileName);
		uint32_t PinIDCounter = 0;
	private:
		
		//���ߡ� ȷ���ڵ��ִ��˳�򣬲���鲻��Ҫ���µĽڵ�
		void SortAndActiveNodes();

		bool Global = true;

		uint32_t NodeIDCounter = 0;
		
		//temp
		entt::entity m_ent;
		Scene* m_sc;
		float m_ts;
		BluePrintProperties& m_prop;


		std::string m_Name;
		std::string m_FileName;

		BluePrintProperties m_DefaultProperties;

		std::vector<XRef<BluePrintField>> m_Fields;
		std::vector<XRef<BluePrintNode>> m_Nodes;

		unordered_map<uint32_t, XRef<InputPin>> m_InputPins;
		unordered_map<uint32_t, XRef<OutputPin>> m_OutputPins;

	

	};

	
	

}