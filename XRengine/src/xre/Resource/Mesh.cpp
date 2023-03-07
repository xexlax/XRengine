#include "pch.h"
#include "Mesh.h"

#include "xre/Renderer/Renderer.h"
#include "Platforms\OpenGL\OpenGLShader.h"

namespace XRE {
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		: m_vertices(vertices), m_indices(indices)
	{
		m_Material = std::make_shared<Material>();
		setupMesh();
	}
	
	void Mesh::BindMaterial(XRef<Shader> shader)
	{
		shader->Bind();
	
		shader->SetFloat3("material.baseColor", m_Material->baseColor);
		shader->SetFloat("material.shininess", m_Material->shininess);
		shader->SetFloat("material.metallic", m_Material->metallic);
		shader->SetFloat("material.roughness", m_Material->roughness);
		int textureID = 1;
		//
		shader->SetBool("material.enable_diffuseTex", m_Material->diffuseTex.m_enable);
		if (m_Material->diffuseTex.m_enable) {
			m_Material->diffuseTex.m_Tex->Bind(textureID);
			
			shader->SetInt("material.diffuseTex", textureID++);
		}
		//
		shader->SetBool("material.enable_specularTex", m_Material->specularTex.m_enable);
		if (m_Material->specularTex.m_enable) {
			m_Material->specularTex.m_Tex->Bind(textureID);
			shader->SetInt("material.specularTex", textureID++);
		}
		//
		shader->SetBool("material.enable_bumpTex", m_Material->bumpTex.m_enable);
		if (m_Material->bumpTex.m_enable) {
			m_Material->bumpTex.m_Tex->Bind(textureID);
			shader->SetInt("material.bumpTex", textureID++);
		}
			
		
	}
	void Mesh::UnBindMatarial(XRef<Shader> shader)
	{
	}
	void Mesh::setupMesh()
	{
		BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float3, "a_Tangent" },
				{ ShaderDataType::Float2, "a_TexCoord" }
		};

		m_VertexArray = VertexArray::Create();

		//2.1 setup VBO
		XRef<VertexBuffer> vertexBuffer;
		float *s = (float*)&m_vertices[0];
		vertexBuffer.reset(VertexBuffer::Create( (float*) &m_vertices[0] ,m_vertices.size()* layout.GetStride()));
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//2.2 setup IBO
		XRef<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create( (uint32_t*) &m_indices[0], m_indices.size()));
		m_VertexArray->SetIndexBuffer(indexBuffer);

	}
}