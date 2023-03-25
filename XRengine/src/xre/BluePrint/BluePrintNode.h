#pragma once

#include "BluePrintField.h"
#include "cereal\archives\json.hpp"

namespace XRE {
	class BluePrint;
	struct InputPin;
	struct OutputPin;
	
	struct InputPin {

		int parent;
		int order;

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
			T v= T();
			if (m_Connection != nullptr) {
				return m_Connection->GetValue<T>();
			}
			else return v;
		}
		


		FieldType m_FieldType;
		OutputPin* m_Connection = nullptr;
	};

	struct OutputPin {

		int parent;
		int order;

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
		int order;
		int parent;
		int field_ptr;
		void* m_Handle=nullptr;

		NodeVariant() = default;

		NodeVariant(FieldType ft, void* handle,std::string name= u8"##") {
			m_Type = ft;
			m_Handle = handle;
			m_Name = name;
		}

		template <typename Archive>
		void save(Archive& ar) const{
			ar(order,parent,m_Name,m_Type);
			switch (m_Type)
			{
			case Field_Bool: {
				bool b = *(bool*)m_Handle;
				ar(b);
				break;
			}
				
			case Field_Int: {
				int b = *(int*)m_Handle;
				ar(b);
				break;
			}
				
			case Field_Float:
			{
				float b = *(float*)m_Handle;
				ar(b);
				break;
			}
			case Field_String:
			{
				string b = *(string*)m_Handle;
				ar(b);
				break;
			}

			case Field_Field:
			{
				ar(field_ptr);
				//ar(**(XRef<BluePrintField>*)(m_Handle));
				break;
			}

			default:
				break;
			}
		}


		template <typename Archive>
		void load(Archive& ar) {
			ar(order, parent, m_Name, m_Type);
			switch (m_Type)
			{
			case Field_Bool: {
				m_Handle = (void*)( new bool());
				ar(*(bool*)m_Handle);
				break;
			}

			case Field_Int: {
				m_Handle = (void*)(new int());
				ar(*(int*)m_Handle);
				break;
			}

			case Field_Float:
			{
				m_Handle = (void*) (new float());
				ar(*(float*)m_Handle);
				break;
			}
			case Field_String:
			{
				m_Handle = static_cast<void*>(new std::string(""));
				ar(*(std::string*)m_Handle);
				break;
			}
			case Field_Field:
			{
				ar(field_ptr);
				//ar(**(XRef<BluePrintField>*)m_Handle);
				break;
			}

			default:
				break;
			}
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
		bool CanFullyProcess();
		bool ControlFlowTest();
		void GetReady();
		void ResetReady();
		virtual void Start() {};
		virtual void Process() = 0;
		virtual void Initialize() = 0;
		virtual void SetField(XRef<BluePrintField> field) {};

		virtual XRef<BluePrintField> GetField() { return nullptr; };
		bool m_Active = true;
		uint32_t NodeID;


		std::string m_Title = "Node";
		int m_NodeTypeID;
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