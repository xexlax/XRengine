#pragma once
#include <glm\glm.hpp>
#include "Material.h"

#include <xre\Renderer\Resources\Shader.h>
#include <xre\Renderer\Resources\VertexArrays.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>


namespace XRE {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec2 TexCoords;
	};
	

	class Mesh
	{
	public:
		
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		
		~Mesh() {};

		void BindMaterial(XRef<Material> mat, XRef<Shader> shader);
		void UnBindMatarial(XRef<Shader> shader);

		XRef<VertexArray> GetVAO()const { return m_VertexArray; }
		uint32_t MatID = 0;
	private:
		
		XRef<VertexArray> m_VertexArray;
		glm::mat4 m_Transform;

		//void BindMaterial(XRef<Shader> shader);
		void setupMesh();
		

	};

}

