#pragma once
#include "core.h"

namespace XRE {
	class DECL Application{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}