#include "pch.h"
#include "Mesh.h"

#include "xre/Renderer/Renderer.h"

namespace XRE {
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Ref<Texture>> textures)
		: m_vertices(vertices), m_indices(indices), m_textures(textures)
	{

		setupMesh();
	}
	void Mesh::Draw(Ref<Shader> shader)
	{
		Renderer::Submit(shader, m_VertexArray, m_Transform);
	}
	void Mesh::setupMesh()
	{
		BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
		};

		m_VertexArray = VertexArray::Create();

		//2.1 setup VBO
		Ref<VertexBuffer> vertexBuffer;
		//vertexBuffer.reset(VertexBuffer::Create(m_vertices, sizeof(m_vertices)));
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//2.2 setup IBO
		Ref<IndexBuffer> indexBuffer;
		//indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

	}
}