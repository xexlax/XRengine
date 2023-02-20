//#pragma once
//#include "xre\Core\Macros.h"
//#include "xre\Renderer\Shader.h"
//#include "xre\Renderer\Buffers.h"
//#include "xre\Renderer\Texture.h"
//#include "pch.h"
//
//namespace XRE{
//	enum MaterialDataType {
//		Int,Float,Tex
//	};
//	class MaterialData {
//	public:
//		MaterialData(std::string name, MaterialDataType type):m_name(name),m_type(type) {};
//		~MaterialData() {};
//		
//		std::string getName() { return m_name; }
//		MaterialDataType getType() { return m_type; }
//		void SetDataValue(void* value, MaterialDataType type) {
//			if (type != m_type) XRE_CORE_ERROR("False material type setting");
//			switch (m_type)
//			{
//			case MaterialDataType::Int:
//				m_IntData = *(int*)value;
//				break;
//			case MaterialDataType::Float:
//				m_FloatData = *(float*)value;
//				break;
//			case MaterialDataType::Tex:
//				m_TextureData = *(Ref<Texture>*)value;
//				break;
//			default:
//				XRE_CORE_ERROR("Unexist material type setting");
//				break;
//			}
//		}
//	private:
//		std::string m_name;
//		MaterialDataType m_type;
//		int m_IntData;
//		float m_FloatData;
//		Ref<Texture> m_TextureData;
//	};
//	class Material
//	{
//	public:
//		//Todo 包含Shader 和变量
//		Material() {};
//		~Material() {};
//
//		//从Shader中加载属性结构
//		void SetShader(Ref<Shader> shader);
//		
//		//导出buffer结构
//		BufferLayout GetBufferLayout();
//		Ref<MaterialData> GetDataRef(std::string name);
//		
//		void Bind();
//
//	private:
//		std::vector<MaterialData> m_datas;
//		Ref<Shader> m_shader;
//	};
//
//}
