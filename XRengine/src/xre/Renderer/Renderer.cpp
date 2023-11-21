#include "pch.h"
#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLShader.h"
#include "Platforms\Vulkan\VkContext.h"

namespace XRE {
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	XRef<LightSystem> Renderer::m_LightSystem;
	void Renderer::Init()
	{
		RenderCommand::Init();
		
		m_LightSystem = XMakeRef<LightSystem>();
		m_SceneData->lightSystem = m_LightSystem;
	}
	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const XRef<Camera>& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		m_SceneData->ViewMatrix = camera->GetViewMatrix();
		m_SceneData->ProjectionMatrix = camera->GetProjectionMatrix();
		

#ifdef XRE_RENDERER_VULKAN
		XRE::VkContext::GetInstance()->beginScene(m_SceneData);
#endif // XRE_RENDERER_VULKAN
	}

	

	void Renderer::EndScene()
	{
#ifdef XRE_RENDERER_VULKAN

		XRE::VkContext::GetInstance()->endScene();
#endif // XRE_RENDERER_VULKAN

	}

	void Renderer::DrawModel(const MeshRendererComponent& mrc, const glm::mat4& transform)
	{

#ifdef XRE_RENDERER_VULKAN
		PushData push{};
		push.ModelMatrix = transform;
		XRE::VkContext::GetInstance()->PushConstant(&push, sizeof(PushData));
		for (auto mesh : mrc.m_Model->m_Meshes) {
			mesh.BindMaterial(mrc.GetMaterial(mesh.MatID));
			
			RenderCommand::DrawIndexed(
				mesh.GetVAO()
			);
		}

#endif
	}


	

	

	void Renderer::Submit(const std::shared_ptr<Shader>& shader,
		const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
#ifdef XRE_RENDERER_OPENGL
		//XRE_CORE_TRACE("VP:{0}", m_SceneData->ViewProjectionMatrix[0][0]);
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
#endif // XRE_RENDERER_OPENGL

#ifdef XRE_RENDERER_VULKAN

#endif // XRE_RENDERER_VULKAN

		
	}

}