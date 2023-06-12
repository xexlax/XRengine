#pragma once
#include <XRE.h>

class XREContext {
public:
	static XREContext* s_XREContext;

	XRE::XRef<XRE::Project> m_Project;
	XRE::XRef<XRE::Scene> m_Scene;


	static void Init() {
		s_XREContext = new XREContext();
		s_XREContext->Initialize();
	}

private:
	void Initialize() {
		m_Project = XRE::XMakeRef<XRE::Project>("..\\Projects\\project2\\project2.prj");
		m_Project->Load();
		XRE::ResourceManager::BindProj(m_Project);

		m_Scene = XRE::XMakeRef<XRE::Scene>();
		m_Scene->Deserialize(m_Project->m_RootPath + "\\Assets\\" + m_Project->m_Scenes[0]);
		m_Scene->OnRuntimeBegin();
	}

};