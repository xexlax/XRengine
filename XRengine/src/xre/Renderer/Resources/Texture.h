#pragma once
#include <string>

#include "xre\Core\Macros.h"
namespace XRE {
	//用于Material中的Texture相关属性
	struct MaterialTex;
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		std::string GetName() const { return m_name; };
		virtual uint32_t GetRendererId() const =0;
	protected:
		std::string m_name="default";
		std::string m_Path;
	};

	class Texture2D : public Texture
	{
	public:
		static XRef<Texture2D> Create(const std::string& path);

	};

	

	struct MaterialTex {
	public:
		bool m_enable = false;
		XRef<Texture2D> m_Tex;
		std::string m_name, m_filepath;
		MaterialTex() {

		}
		MaterialTex(bool enable, std::string name, std::string filepath) :
			m_enable(enable), m_name(name), m_filepath(filepath) {

		}

		operator std::string& () {
			return m_filepath;
		}

		operator const std::string& () const {
			return m_filepath;
		}


	};

	

}