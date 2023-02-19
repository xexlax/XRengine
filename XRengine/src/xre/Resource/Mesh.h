#pragma once
#include <glm\glm.hpp>
#include <xre\Renderer\Texture.h>
#include <xre\Renderer\Shader.h>
#include <xre\Renderer\VertexArrays.h>

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
		
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Ref<Texture>> textures);
		~Mesh() {};
		void Draw(Ref<Shader> shader);
	private:
		
		Ref<VertexArray> m_VertexArray;
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<Ref<Texture>> m_textures;
		glm::mat4 m_Transform;


		void setupMesh();

	};

}

