#pragma once
#include <xre\Renderer\Resources\Shader.h>

#include <xre\Renderer\Resources\Texture.h>

#include "xre\Core\Macros.h"
#include <xre\Renderer\Camera\Camera.h>
#include <xre\Resource\Model.h>

#include <xre/Resource/ResourceManager.h>
#include <xre\Renderer\Light.h>
#include <xre\Renderer\SkyBox.h>

namespace XRE {
	class Renderer3D {
	public:
		static void Init();
		static void ShutDown();
		static void StartScene(const Ref<Camera> camera);
		static void EndScene();
		static void PBROn(bool on);

		static void DrawLight(const Ref<Light> light);
		static void DrawModel(const Ref<Model> model, glm::mat4 transform = glm::mat4(1.0f));
		static void DrawSkybox(const Ref<SkyBox> skybox);

		static Ref<Shader> defaultObjShader;
		static Ref<Shader> defaultPBRShader;
		static Ref<Texture2D> defaultAlbedo;

		static Ref<Shader> activeShader;
		static glm::mat4 m_VP;

	};	
}