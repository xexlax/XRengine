#include "pch.h"
#include "Material.h"

namespace XRE {
	void Material::SetShader(Ref<Shader> shader)
	{
	}
	BufferLayout Material::GetBufferLayout()
	{
		return BufferLayout();
	}
	Ref<MaterialData> Material::GetDataRef(std::string name)
	{
		return Ref<MaterialData>();
	}
	void Material::Bind()
	{
	}
}