#pragma once
#include <xre\Renderer\Resources\Shader.h>
#include <xre\Renderer\Resources\Texture.h>

#include "xre\Core\Macros.h"
#include <xre\Renderer\Camera\Camera.h>
#include <xre\Resource\Model.h>

#include <xre/Resource/ResourceManager.h>

#include <xre\Renderer\FrameBuffer.h>
#include <xre\Renderer\Light.h>
#include <xre\Renderer\SkyBox.h>
#include "xre\ECS\Components.h"
namespace XRE {
	class Renderer3D {
	public:
		static void Init();
		static void ShutDown();
		//通用渲染的前置与后置
		static void StartScene(const Ref<Camera> camera);
		static void EndScene();


		


		//渲染ShadowMap的前置与后置
		static void StartShadowPass();
		static void EndShadowPass();


		static void SetShadowMapOfActive(uint32_t slot=0);
		static void PostProcessing();

		//Commands
		static void Clear();
		static void SetClearColor(glm::vec4 color);
		static void CullFace(bool b);

		//将m_Light中的灯光数据绑定到activeShader
		static void DrawLight();

		static void DrawModel(const Ref<Model> model, glm::mat4 transform = glm::mat4(1.0f));
		static void DrawSkybox();

		static Ref<Shader> defaultObjShader;
		static Ref<Shader> defaultPBRShader;
		static Ref<Shader> simpleDepthShader;
		static Ref<Shader> postShader;

		static Light m_Light;
	
		
		static Ref<Framebuffer> m_FrameBuffer,m_ShadowFrameBuffer, m_PostFrameBuffer;
		static Ref<SkyBox> m_SkyBox;

		static Ref<Shader> activeShader;

		static Ref<Texture2D> defaultAlbedo;

		//暂时性储存
		static glm::mat4 m_VP,m_LVP;
		static Ref<VertexArray> m_Quad;

	};	
}