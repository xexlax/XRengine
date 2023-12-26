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
		//Todo 包含Shader 和变量
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
		float metallic=0, roughness=0;
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
			ar(name,m_FilePath, baseColor.x, baseColor.y, baseColor.z,
				shininess, ior, dissolve, pad0, metallic, roughness, illum,
				ambientTex, diffuseTex, specularTex, specularHighlightTex,
				bumpTex, displacementTex, alphaTex, ambientOcclusionTex);
		}

		void Save();
		void Save(const std::string& filepath);
		void Load(const std::string& filepath);

		virtual void Bind() {};

		std::string& GetPath() { return m_FilePath; }
		std::string m_FilePath = "";
		void SetPath(std::string& s) { m_FilePath = s; }
	private:
		
		
		
	};

}
