#pragma once
#include "xre\Renderer\Resources\Shader.h"
#include <vulkan\vulkan.h>
namespace XRE {
	class VulkanShader : public Shader {
	public:
		VulkanShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
		VulkanShader(const std::string& path);
		virtual ~VulkanShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual const std::string& GetName() const override { return m_Name; }

		void SetBool(const std::string& name, bool value);
		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat3(const std::string& name, const glm::mat3& matrix);
		void SetMat4(const std::string& name, const glm::mat4& matrix);

		VkShaderModule vert, frag;



	private:
		std::string m_Name;


		//for logging errors
		std::string m_VertexPath, m_FragmentPath;

		VkShaderModule createShaderModule(const std::vector<char>& code);

		static std::vector<char> readFile(const std::string& filename) {
			std::ifstream file(filename, std::ios::ate | std::ios::binary);

			if (!file.is_open()) {
				XRE_CORE_ERROR("failed to open file!");
			}

			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();

			return buffer;
		}
	};
}