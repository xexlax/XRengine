
#include "VulkanVertexArray.h"
#include "VulkanBuffers.h"

#include <tiny_obj_loader.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include "VulkanSwapChain.h"
#include "VkContext.h"
namespace std {
    template<> struct hash<XRE::VkVertex> {
        size_t operator()(XRE::VkVertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

XRE::VulkanVertexArray::VulkanVertexArray()
{
}

XRE::VulkanVertexArray::~VulkanVertexArray()
{
}

void XRE::VulkanVertexArray::Bind() const
{
}

void XRE::VulkanVertexArray::Unbind() const
{
}

void XRE::VulkanVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	m_VertexBuffers.push_back(vertexBuffer);
}

void XRE::VulkanVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	m_IndexBuffer = indexBuffer;
    indexCount = indexBuffer->GetCount();
}

void XRE::VulkanVertexArray::Bind(VkCommandBuffer commandBuffer)
{

    int buffercount = m_VertexBuffers.size();

    VkBuffer *vertexBuffers= new VkBuffer[buffercount];
    
    for (int i = 0; i < buffercount; i++) {
        vertexBuffers[i] = std::dynamic_pointer_cast<VulkanVertexBuffer>(m_VertexBuffers[i])->m_vertexBuffer;
    }
    
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, std::dynamic_pointer_cast<VulkanIndexBuffer>(m_IndexBuffer) ->m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);

}

void XRE::VulkanVertexArray::Draw(VkCommandBuffer commandBuffer)
{
    vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
}

void XRE::VulkanVertexArray::UpdateDescriptorSets(const std::vector<XRef<Texture2D>>& textures)
{
    if (descriptorWriter == nullptr) createDescriptorSets(textures);
    /*else
    for (int i = 0;i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;i++) {
        descriptorWriter->writeImage(std::dynamic_pointer_cast<VulkanTexture2D>(textures[0])->m_Image, 1);
        descriptorWriter->overwrite(i);
    }*/

   

}

void XRE::VulkanVertexArray::createDescriptorSets(const std::vector<XRef<Texture2D>>& textures)
{
    descriptorWriter = XMakeRef<VulkanDescriptorWriter>(VkContext::GetInstance()->modelPipeline);
    for (int i = 0;i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;i++) {
        descriptorWriter->writeBuffer(VkContext::GetInstance()->modelPipeline->GlobalUBOs[i]);
        descriptorWriter->writeBuffer(VkContext::GetInstance()->modelPipeline->LightingUBOs[i]);
        descriptorWriter->writeImage( std::dynamic_pointer_cast<VulkanTexture2D>(textures[0])->m_Image);
        descriptorWriter->writeImage(std::dynamic_pointer_cast<VulkanTexture2D>(textures[0])->m_Image);
        descriptorWriter->writeImage(std::dynamic_pointer_cast<VulkanTexture2D>(textures[0])->m_Image);
        descriptorWriter->overwrite(i);
    }
}


