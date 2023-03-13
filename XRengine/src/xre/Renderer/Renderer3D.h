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
		//ͨ����Ⱦ��ǰ�������
		static void StartScene(const XRef<Camera> camera);
		static void EndScene();


		


		//��ȾShadowMap��ǰ�������
		static void StartShadowPass();
		static void EndShadowPass();


		static void SetShadowMapOfActive(uint32_t slot=0);
		static void PostProcessing();

		//Commands
		static void Clear();
		static void SetClearColor(glm::vec4 color);
		static void CullFace(bool b);

		//��m_Light�еĵƹ����ݰ󶨵�activeShader
		static void DrawLight();

		static void DrawMesh(const XRef<Model> model, const glm::mat4& transform);

		static void DrawModel(const XRef<Model> model, const std::vector<XRef<Material>>& mats, const glm::mat4& transform = glm::mat4(1.0f));
		static void DrawSkybox();

		static void DrawShapeFrame(const PhysicsShape& Shape, const glm::mat4& transform = glm::mat4(1.0f), const glm::vec4& color = glm::vec4(0, 1, 0, 1));
		static void DrawRay(const glm::vec3& origin, const glm::vec3& direction, float length, const glm::vec4& color=glm::vec4(1,0,0,1) );

		static XRef<Shader> defaultObjShader;
		static XRef<Shader> defaultPBRShader;
		static XRef<Shader> simpleDepthShader;
		static XRef<Shader> flatColorShader;
		static XRef<Shader> postShader;

		static Light m_Light;
	
		
		static XRef<Framebuffer> m_FrameBuffer,m_ShadowFrameBuffer, m_PostFrameBuffer;
		static XRef<SkyBox> m_SkyBox;

		static XRef<Shader> activeShader;

		static XRef<Texture2D> defaultAlbedo;

		//��ʱ�Դ���
		static glm::mat4 m_VP,m_LVP;
		static XRef<VertexArray> m_Quad;

	};	
}