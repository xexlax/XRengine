#pragma once
#include <glm\glm.hpp>
#include "Material.h"

#include <xre\Renderer\Resources\Shader.h>
#include <xre\Renderer\Resources\VertexArrays.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <xre\Utils\Vertex.h>

namespace XRE {

	
	struct AABB {
		glm::vec3 LowerBorder, HigherBorder;
	};
	

	class Mesh
	{
	public:
		
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		
		~Mesh() {};

		void BindMaterial(XRef<Material> mat, XRef<Shader> shader);

		void BindMaterial(XRef<Material> mat);
		void UnBindMatarial(XRef<Shader> shader);

		XRef<VertexArray> GetVAO()const { return m_VertexArray; }
		uint32_t MatID = 0;
		AABB m_AABB;
	private:
		
		XRef<VertexArray> m_VertexArray;
		glm::mat4 m_Transform;
		
		//void BindMaterial(XRef<Shader> shader);
		void setupMesh();
		

	};

}

