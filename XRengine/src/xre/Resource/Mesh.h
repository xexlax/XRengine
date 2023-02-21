#pragma once
#include <glm\glm.hpp>
#include "Material.h"

#include <xre\Renderer\Shader.h>
#include <xre\Renderer\VertexArrays.h>
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
		
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		
		~Mesh() {};

		void Draw(Ref<Shader> shader,glm::mat4 transform= glm::mat4(1.0f));
		void SetMaterial(Ref<Material> mat) { m_Material = mat; };

		
	private:
		
		Ref<VertexArray> m_VertexArray;
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		Ref<Material> m_Material;
		glm::mat4 m_Transform;

		void BindMaterial(Ref<Shader> shader);
		void setupMesh();

	};

}

