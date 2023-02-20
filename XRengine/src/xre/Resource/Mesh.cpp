#include "pch.h"
#include "Mesh.h"

#include "xre/Renderer/Renderer.h"


namespace XRE {
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		: m_vertices(vertices), m_indices(indices)
	{

		setupMesh();
	}
	void Mesh::Draw(Ref<Shader> shader, glm::mat4 transform)
	{
		
		Renderer::Submit(shader, m_VertexArray,transform);
	}
	void Mesh::setupMesh()
	{
		BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float2, "a_TexCoord" }
		};

		m_VertexArray = VertexArray::Create();

		//2.1 setup VBO
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create( (float*) &m_vertices[0] , sizeof((float*)&m_vertices[0])));
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//2.2 setup IBO
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create( (uint32_t*) &m_indices[0], m_indices.size()));
		m_VertexArray->SetIndexBuffer(indexBuffer);

	}
}