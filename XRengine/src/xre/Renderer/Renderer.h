#pragma once

#include "RenderCommand.h"
#include "Camera/Camera.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Light.h"
namespace XRE{

	

	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(const XRef<Camera>& camera);
		static void EndScene();

		static void DrawModel(const XRef<Model>& model);

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray
			, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		
		//Global UBO
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;

			XRef<LightSystem> lightSystem;
		};
	private:
		static SceneData* m_SceneData;
		
	};


}