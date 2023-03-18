#pragma once

namespace XRE {

	enum FieldType {
		None, Int, Float, Bool, Vec3, Vec4, Mat4
	};


	class BluePrintField {

	

	public:
		static XRef<BluePrintField> Create(std::string name, FieldType type);

		~BluePrintField() = default;


	private:
		std::string m_FieldName;

		FieldType m_Type;
		

	};


	template<typename T>
	class BluePrintFieldImpl: public BluePrintField {
	public:
		T Get()const { return m_Value; }
		void Set(const T& v) { m_Value = v; }

	private:
		T m_Value;

	};
}