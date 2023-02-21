#pragma once
#include <string>

#include "xre\Core\Macros.h"
namespace XRE {
	struct MaterialTex;
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		std::string GetName() const { return m_name; };
	protected:
		std::string m_name="default";
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);

	};

	//管理所有的贴图，防止重复加载
	class Texture2DLibrary {
	public:
		void Add(const std::string& name, const Ref<Texture2D>& texture);
		void Add(const Ref<Texture2D>& texture);
		Ref<Texture2D> Load(const std::string& filepath);
		Ref<Texture2D> Load(const std::string& name, const std::string& filepath);
		Ref<Texture2D> Load(MaterialTex &materialTex);
		Ref<Texture2D> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:

		std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
	};

	struct MaterialTex {
	public:
		bool m_enable = false;
		Ref<Texture2D> m_Tex;
		std::string m_name, m_filepath;
		MaterialTex() {

		}
		MaterialTex(bool enable, std::string name, std::string filepath) :
			m_enable(enable), m_name(name), m_filepath(filepath) {

		}


	};

	

}