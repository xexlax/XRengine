#include "PanelsManager.h"
namespace XRE {
	XRef<PanelsManager> PanelsManager::instance;
	void PanelsManager::Init()
	{
		instance = XMakeRef<PanelsManager>();
	}
}
