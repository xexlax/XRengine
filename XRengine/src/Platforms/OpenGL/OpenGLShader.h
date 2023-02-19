#pragma once
#include "xre\Renderer\Shader.h"
namespace XRE {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
		OpenGLShader(const std::string& path);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual const std::string& GetName() const override { return m_Name; }

		void setBool(const std::string& name, bool value);
		void setInt(const std::string& name, int value);
		void setFloat(const std::string& name, float value);
		void setFloat2(const std::string& name,const glm::vec2& value);
		void setFloat3(const std::string& name,const glm::vec3& value);
		void setFloat4(const std::string& name,const glm::vec4& value);
		void setMat3(const std::string& name, const glm::mat3& matrix);
		void setMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filepath);
		void PreProcess(const std::string& source, std::string& vc, std::string& fc);
		void Compile(const std::string& vc, const std::string& fc );

	private:
		uint32_t m_RendererID;
		std::string m_Name;

		void checkCompileErrors(unsigned int shader, std::string type);
		//for logging errors
		std::string m_VertexPath, m_FragmentPath;

		
	};
}