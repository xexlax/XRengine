#pragma once
#include "xre\Core\Macros.h"
#include "xre\Renderer\Shader.h"
#include "xre\Renderer\Buffers.h"
#include "xre\Renderer\Texture.h"
#include "pch.h"

namespace XRE{
	
	class Material
	{
	public:
		//Todo ����Shader �ͱ���
		Material() {};
		~Material() {};

		std::string name;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 transmittance;
		glm::vec3 emission;
		float shininess, ior, dissolve, pad0;
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
