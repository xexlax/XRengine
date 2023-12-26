
#include "VulkanTexture.h"
#include "VkContext.h"
#include "xre/Utils/image.h"
//#include "vk_mem_alloc.h"
#include "VulkanRHI.h"


XRE::VulkanTexture2D::VulkanTexture2D(const std::string& path)
{
    m_Path = path;
    createTextureImage();
    createTextureImageView();
    createTextureSampler();
}

XRE::VulkanTexture2D::~VulkanTexture2D()
{
    
    //ImGui_ImplVulkan_RemoveTexture(ds);

    
}

uint32_t XRE::VulkanTexture2D::GetRendererId() { 
    
    return uint32_t(GetDescriptor());

}

void* XRE::VulkanTexture2D::GetDescriptor()
{

    return m_Image->GetDescriptorSet();
    
}

void XRE::VulkanTexture2D::Bind(uint32_t slot) const
{

}

void XRE::VulkanTexture2D::createTextureImage()
{
    int texWidth, texHeight, texChannels;

    unsigned char* pixels= load_image( m_Path.c_str(), &texWidth, &texHeight, &texChannels, 4, true);

    m_Width = texWidth;
    m_Height = texHeight;

    VkDeviceSize imageSize = texWidth * texHeight* 4;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

    VkDevice device = VkContext::GetDevice();
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanRHI::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);



    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);

    
        memcpy(data, pixels, static_cast<size_t>(imageSize));
    
    vkUnmapMemory(device, stagingBufferMemory);

    VkFormat format= VK_FORMAT_R8G8B8A8_SRGB;
    /*if (texChannels == 3) {
        format = VK_FORMAT_R8G8B8_SRGB;
    }
    else if (texChannels == 4) {
        format = VK_FORMAT_R8G8B8A8_SRGB;
    }*/
    image_free(pixels);
    m_Image = XMakeRef<VulkanImage>();
    m_Image->Create(texWidth, texHeight, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    m_Image->transitionImageLayout( format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    m_Image->copyBufferToImage(stagingBuffer, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    m_Image->transitionImageLayout(format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void XRE::VulkanTexture2D::createTextureImageView() {
    m_Image->ImageView = VulkanRHI::createImageView(m_Image->Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

void XRE::VulkanTexture2D::createTextureSampler() {
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(VkContext::GetInstance()->physicalDevice, &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    if (vkCreateSampler(VkContext::GetInstance()->device, &samplerInfo, nullptr, &m_Image->Sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

