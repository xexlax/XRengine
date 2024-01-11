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
#include "xre\Physics\PhysicsShape.h"
namespace XRE {
	class Renderer3D {
	public:
		static void Init();
		static void ShutDown();
		//通用渲染的前置与后置
		static void StartScene(const XRef<Camera> camera);
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

		static void DrawMesh(const XRef<Model> model, const glm::mat4& transform);

		//static unsigned int quadVAO, quadVBO;
		static void DrawScreenQuad();

		static void DrawModel(const MeshRendererComponent& mrc, const glm::mat4& transform = glm::mat4(1.0f));
		static void DrawSprite(const SpriteRendererComponent& mrc, const glm::mat4& transform = glm::mat4(1.0f));

		
		static void DrawSkybox();

		static void DrawShapeFrame(const PhysicsShape& Shape, const glm::mat4& transform = glm::mat4(1.0f), const glm::vec4& color = glm::vec4(0, 1, 0, 1));
		static void DrawRay(const glm::vec3& origin, const glm::vec3& direction, float length, const glm::vec4& color=glm::vec4(1,0,0,1) );

		static XRef<Shader> defaultObjShader;
		static XRef<Shader> defaultPBRShader;
		static XRef<Shader> simpleDepthShader;
		static XRef<Shader> flatColorShader;


		static XRef<Shader> SSAO_Shader;
		static XRef<Shader> SSR_Shader;
		static XRef<Shader> postShader;

		static XRef<Shader> DeferredShader;

		static LightSystem m_Light;

		static bool SSAO_ON,SSR_ON;

		static struct PostEffects{
			float Brightness=1;
			float Saturation=1;
			float Contrast=1;
			float Vignette;

		} postEffects;
	
		
		static XRef<Framebuffer> m_FrameBuffer,m_DeferredFrameBuffer,m_ShadowFrameBuffer, m_PostFrameBuffer , m_SSAOBuffer, m_SSRBuffer;
		static XRef<SkyBox> m_SkyBox;

		static XRef<Shader> activeShader;

		static XRef<Texture2D> defaultAlbedo,ssaoNoise;

		//暂时性储存
		static glm::mat4 m_VP,m_LVP;
		static XRef<VertexArray> m_Quad;

	};	
}