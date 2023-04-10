#include <XRE.h>
#include <xre/Core/entryPoint.h>
#include <imgui.h>
using namespace XRE;
class LauncherLayer : public XRE::Layer
{

	XRef<Project> m_Project;
	XRef<Scene> m_Scene;

	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportBounds[2];


	uint32_t windowsSizeX= 1920, windowsSizeY= 1080;
public:
	LauncherLayer() {};
	virtual ~LauncherLayer() = default;

	virtual void OnAttach() override {
		m_Project = XMakeRef<Project>("..\\Projects\\project2\\project2.prj");
		m_Project->Load();
		ResourceManager::BindProj(m_Project);
		Renderer3D::Init();
		m_Scene = XMakeRef<Scene>();
		m_Scene->Deserialize(m_Project->m_RootPath + "\\Assets\\" + m_Project->m_Scenes[0]);
		m_Scene->OnViewportResize(windowsSizeX, windowsSizeY);
		m_Scene->OnRuntimeBegin();
		
	};
	virtual void OnDetach() override {
		m_Scene->OnRuntimeEnd();
		Renderer3D::ShutDown();
	};

	void OnUpdate(TimeStep ts) override {
		
		m_Scene->OnUpdateRuntime(ts);
	}


	void LauncherLayer::OnImGuiRender() {



		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		ImGui::Begin(u8"ÊÓ´°");


		//ToolBar();

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			//XRE_CORE_TRACE("viewport: {0} x {1} ", viewportPanelSize.x, viewportPanelSize.y);
			Renderer3D::m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		uint32_t textureID = Renderer3D::m_FrameBuffer->GetColorAttachment(0);
		ImGui::Image((ImTextureID)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	
		ImGui::End();
		ImGui::PopStyleVar();
	}

	


};


class LauncherApp: public XRE::Application
{
public:
	LauncherApp();
	~LauncherApp();

private:

};

LauncherApp::LauncherApp()
{
	PushLayer(new LauncherLayer());	
}

LauncherApp::~LauncherApp()
{
}

XRE::Application* XRE::CreateApplication() {
	return new LauncherApp();
}
