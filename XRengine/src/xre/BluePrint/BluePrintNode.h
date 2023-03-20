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
		InputPin(FieldType field = FieldType::Field_Blank, std::string m_Name = "Input") {
			m_FieldType = field;
		}
		template <typename T>
		T GetValue() {
			T v= default;
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
		OutputPin(FieldType field = FieldType::Field_Blank, std::string m_Name = "Output") {
			m_FieldType = field;
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

	class BluePrintNode {
	public:
		BluePrintNode() {};
		
		bool NeedProcess();
		bool CanProcess();
		void GetReady();
		void ResetReady();
		virtual void Process() = 0;
		bool m_Active = true;
		uint32_t NodeID;
		std::string m_Title = "Node";
		std::vector<XRef<InputPin>> m_Inputs;
		std::vector<XRef<OutputPin>> m_Outputs;
	protected:
		
		BluePrint* m_BluePrint;
	
	};

}