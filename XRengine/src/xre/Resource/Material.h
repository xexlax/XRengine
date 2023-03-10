#pragma once
#include "xre\Core\Macros.h"
#include "xre\Renderer\Resources\Shader.h"
#include "xre\Renderer\Resources\Buffers.h"
#include "xre\Renderer\Resources\Texture.h"
#include "cereal\archives\json.hpp"
#include "pch.h"

namespace XRE{
	
	class Material
	{
	public:
		//Todo ????Shader ?ͱ???
		Material();
		Material(std::string s);
		~Material() {};

		glm::vec3 baseColor=glm::vec3(1.0f);

		std::string name;
		glm::vec3 ambient=glm::vec3(0.0f);
		glm::vec3 diffuse = glm::vec3(0.5f);
		glm::vec3 specular = glm::vec3(0.5f);
		glm::vec3 transmittance = glm::vec3(0.0f);
		glm::vec3 emission = glm::vec3(0.0f);
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
		MaterialTex	ambientOcclusionTex;
		
		void LoadAllTex();


		

		static XRef<Material> Create(const std::string& filepath);


		template <class Archive>
		void serialize(Archive& ar)
		{
			ar(name, baseColor.x, baseColor.y, baseColor.z,
				shininess, ior, dissolve, pad0, metallic, roughness, illum,
				ambientTex, diffuseTex, specularTex, specularHighlightTex,
				bumpTex, displacementTex, alphaTex, ambientOcclusionTex);
		}

		void Save() { Save(m_FilePath); }
		void Save(const std::string& filepath);
		void Load(const std::string& filepath);

		std::string GetPath() const { return m_FilePath; }
	private:
		
		std::string m_FilePath = "";
		
	};

}
