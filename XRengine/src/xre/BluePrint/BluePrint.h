#pragma once
#include <vector>
#include "xre\Core\Macros.h"
#include "BluePrintNode.h"
#include "BluePrintField.h"

namespace XRE {
	class BluePrint {


	public:
		BluePrint() = default;
		~BluePrint();
		void OnUpdate();

		void Save();

		static XRef<BluePrint> Create(std::string m_FileName);

	private:
		bool Global = true;
		uint32_t Owner;// -1

		std::string m_Name;
		std::string m_FileName;


		std::vector<XRef<BluePrintField>> m_Fields;
		std::vector<XRef<BluePrintNode>> m_Nodes;

	};
}