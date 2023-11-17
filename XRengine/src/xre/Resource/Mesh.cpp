#include "pch.h"
#include "Mesh.h"

#include "xre/Renderer/Renderer.h"
#include "Platforms\OpenGL\OpenGLShader.h"
#include "Platforms\Vulkan\VulkanVertexArray.h"
#include "xre\Resource\ResourceManager.h"

namespace XRE {
	Mesh::Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indices)
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
		float* s = (float*)&vertices[0];
		vertexBuffer.reset(VertexBuffer::Create((float*)&vertices[0], vertices.size() * layout.GetStride()));
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//2.2 setup IBO
		XRef<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create((uint32_t*)&indices[0], indices.size()));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}
	
	void Mesh::BindMaterial(XRef<Material> mat, XRef<Shader> shader)
	{

#ifdef XRE_RENDERER_OPENGL
		if (mat == nullptr) {
			BindMaterial(ResourceManager::GetDefaultMaterial(), shader);
			return;
	}


		shader->Bind();

		shader->SetFloat3("material.baseColor", mat->baseColor);
		shader->SetFloat("material.shininess", mat->shininess);
		shader->SetFloat("material.metallic", mat->metallic);
		shader->SetFloat("material.roughness", mat->roughness);
		int textureID = 1;
		//
		shader->SetBool("material.enable_diffuseTex", mat->diffuseTex.m_enable);
		if (mat->diffuseTex.m_enable) {
			mat->diffuseTex.m_Tex->Bind(textureID);

			shader->SetInt("material.diffuseTex", textureID++);
		}
		//
		shader->SetBool("material.enable_specularTex", mat->specularTex.m_enable);
		if (mat->specularTex.m_enable) {
			mat->specularTex.m_Tex->Bind(textureID);
			shader->SetInt("material.specularTex", textureID++);
		}
		//
		shader->SetBool("material.enable_bumpTex", mat->bumpTex.m_enable);
		if (mat->bumpTex.m_enable) {
			mat->bumpTex.m_Tex->Bind(textureID);
			shader->SetInt("material.bumpTex", textureID++);
		}
#endif // XRE_RENDERER_OPENGL




	}
	void Mesh::BindMaterial(XRef<Material> mat)
	{
#ifdef XRE_RENDERER_VULKAN
		std::vector < XRef<Texture2D>> texs;
		texs.push_back(mat->diffuseTex.m_Tex);
		texs.push_back(mat->specularTex.m_Tex);
		texs.push_back(mat->bumpTex.m_Tex);
		std::dynamic_pointer_cast<VulkanVertexArray>(GetVAO())->UpdateDescriptorSets(texs);


#endif // XRE_RENDERER_VULKAN
	}
	void Mesh::UnBindMatarial(XRef<Shader> shader)
	{
	}
	void Mesh::setupMesh()
	{
		

	}
}