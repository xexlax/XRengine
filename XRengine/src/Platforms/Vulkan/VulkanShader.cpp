#include "VulkanShader.h"
#include "VkContext.h"

XRE::VulkanShader::VulkanShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
{

    auto vertShaderCode = readFile("shaders/vert.spv");
    auto fragShaderCode = readFile("shaders/frag.spv");

    vert  = createShaderModule(vertShaderCode);
    frag  = createShaderModule(fragShaderCode);
}

XRE::VulkanShader::VulkanShader(const std::string& path)
{
}

XRE::VulkanShader::~VulkanShader()
{
    vkDestroyShaderModule(VkContext::GetInstance()->device, frag, nullptr);
    vkDestroyShaderModule(VkContext::GetInstance()->device, vert, nullptr);
}



void XRE::VulkanShader::Bind() const
{
}

void XRE::VulkanShader::UnBind() const
{
}

void XRE::VulkanShader::SetBool(const std::string& name, bool value)
{
}

void XRE::VulkanShader::SetInt(const std::string& name, int value)
{
}

void XRE::VulkanShader::SetFloat(const std::string& name, float value)
{
}

void XRE::VulkanShader::SetFloat2(const std::string& name, const glm::vec2& value)
{
}

void XRE::VulkanShader::SetFloat3(const std::string& name, const glm::vec3& value)
{
}

void XRE::VulkanShader::SetFloat4(const std::string& name, const glm::vec4& value)
{
}

void XRE::VulkanShader::SetMat3(const std::string& name, const glm::mat3& matrix)
{
}

void XRE::VulkanShader::SetMat4(const std::string& name, const glm::mat4& matrix)
{
}


inline VkShaderModule XRE::VulkanShader::createShaderModule(const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(VkContext::GetInstance()->device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}
