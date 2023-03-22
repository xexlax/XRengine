#pragma once

#include "BluePrintField.h"

namespace XRE {
	class BluePrint;
	struct InputPin;
	struct OutputPin;
	
	struct InputPin {


		uint32_t PinID;
		bool m_Necessary=true;
		bool m_Ready=false;
		std::string m_Name;
		InputPin(FieldType field = FieldType::Field_Blank, std::string name = u8" ‰»Î") {
			m_FieldType = field;
			m_Name = name;
		}
		~InputPin();
		template <typename T>
		T GetValue() {
			T v;
			if (m_Connection != nullptr) {
				return m_Connection->GetValue<T>();
			}
			else return v;
		}
		


		FieldType m_FieldType;
		OutputPin* m_Connection = nullptr;
	};

	struct OutputPin {
		uint32_t PinID;
		bool m_Necessary;
		std::string m_Name;
		OutputPin(FieldType field = FieldType::Field_Blank, std::string name = u8" ‰≥ˆ") {
			m_FieldType = field;
			m_Name = name;
		}
		~OutputPin() {
			for (auto x : m_Connections) {
				x->m_Connection = nullptr;
			}
		}

		FieldType m_FieldType;
		
		bool ValueBool = false;
		int ValueInt =0;
		float ValueFloat = 0.0f;
		std::string ValueString= "";

		template <typename T>
		T GetValue();

		

		std::vector<InputPin*> m_Connections;
	};

	struct NodeVariant {
		std::string m_Name;
		FieldType m_Type;
		void* m_Handle;

		NodeVariant(FieldType ft, void* handle,std::string name= u8"##") {
			m_Type = ft;
			m_Handle = handle;
			m_Name = name;
		}
	};

	class BluePrintNode {
	public:
		enum NodeColor {
			Blue,Grey,Red,Pink,Orange,Green,Yellow, Purple
		}m_Color;

		BluePrintNode() {};

		
		bool NeedProcess();
		bool CanProcess();
		bool ControlFlowTest();
		void GetReady();
		void ResetReady();
		virtual void Process() = 0;
		virtual void Initialize() = 0;
		virtual void SetField(XRef<BluePrintField> field) {};

		virtual XRef<BluePrintField> GetField() { return nullptr; };
		bool m_Active = true;
		uint32_t NodeID;


		std::string m_Title = "Node";
		XRef<InputPin> m_FlowPrev = nullptr;
		XRef<OutputPin> m_FlowProc = nullptr;
		std::vector<XRef<InputPin>> m_Inputs;
		std::vector<XRef<OutputPin>> m_Outputs;
		std::vector<NodeVariant> m_Variants;
		BluePrint* m_BluePrint;
	protected:

		void CreateControlFlow();

		
	};

}