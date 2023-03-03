#pragma once
#include "xre\Core\Macros.h"
#include "xre\Renderer\Resources\Shader.h"
#include "xre\Renderer\Resources\Buffers.h"
#include "xre\Renderer\Resources\Texture.h"
#include "pch.h"

namespace XRE{
	
	class Material
	{
	public:
		//Todo 包含Shader 和变量
		Material();
		~Material() {};

		glm::vec3 baseColor=glm::vec3(1.0f);

		std::string name;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 transmittance;
		glm::vec3 emission;
		float shininess, ior, dissolve, pad0;
		float metallic, roughness;
		int illum;

		MaterialTex	ambientTex;
		MaterialTex	diffuseTex;
		MaterialTex	specularTex;
		MaterialTex	specularHighlightTex;
		MaterialTex	bumpTex;
		MaterialTex	displacementTex;
		MaterialTex	alphaTex;
		
		void LoadAllTex();
	private:
		
		
		
	};

}
