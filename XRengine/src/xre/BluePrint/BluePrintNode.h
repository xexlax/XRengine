#pragma once

#include "BluePrintField.h"

namespace XRE {

	struct Pin {

		
		enum IOType{ None,In, Out}m_IO;

		Pin(IOType io, FieldType field = FieldType::None) {
			m_IO = io;
			m_FieldType = field;
		}

		FieldType m_FieldType;
		Pin* m_Connection = nullptr;
	};

	class BluePrintNode {
	public:
		BluePrintNode() {};

		virtual bool CanProcess() = 0;
		virtual void Process() = 0;

		uint32_t NodeID;
	};

	class Node_Entry :BluePrintNode {
		Pin Next= Pin(Pin::IOType::Out);
	};
}