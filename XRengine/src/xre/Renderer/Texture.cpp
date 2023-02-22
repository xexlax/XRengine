#include "pch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLTexture.h"

namespace XRE {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    XRE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
		}

		XRE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void Texture2DLibrary::Add(const std::string& name, const Ref<Texture2D>& texture)
	{
		//XRE_CORE_ASSERT(!Exists(name), "Texture already exists!");

		if(!Exists(name))
		m_Textures[name] = texture;
	}

	void Texture2DLibrary::Add(const Ref<Texture2D>& texture)
	{
		Add(texture->GetName(), texture);
	}

	Ref<Texture2D> Texture2DLibrary::Load(const std::string& filepath)
	{
		auto tex2d = Texture2D::Create(filepath);
		Add(tex2d);
		return tex2d;
	}

	Ref<Texture2D> Texture2DLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto tex2d = Texture2D::Create(filepath);
		Add(name,tex2d);
		return tex2d;
	}

	Ref<Texture2D> Texture2DLibrary::Load(MaterialTex& materialTex)
	{
		materialTex.m_Tex = Load(materialTex.m_name, materialTex.m_filepath);
		return materialTex.m_Tex;
	}

	Ref<Texture2D> Texture2DLibrary::Get(const std::string& name)
	{
		XRE_CORE_ASSERT(Exists(name), "Texture not found!");
		return m_Textures[name];
	}

	bool Texture2DLibrary::Exists(const std::string& name) const
	{
		return m_Textures.find(name) != m_Textures.end();
	}

}