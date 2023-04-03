#pragma once

namespace XRE {

	enum FieldType {
		Field_Blank, Field_Int, Field_Float, Field_Bool, Field_String, Field_Field,
		Field_Array, Field_Array_Int, Field_Array_Float, Field_Array_Bool, Field_Array_String,
	};


	struct BluePrintField {

		std::string m_FieldName;
		uint32_t id;
		bool m_Visible;
		FieldType m_Type;

		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(m_FieldName, m_Type, m_Visible);
		}

	};

}