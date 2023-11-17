#include "pch.h"
#include "VulkanBuffers.h"
#include "VkContext.h"
#include "VulkanRHI.h"

XRE::VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t size)
{

    VkDeviceSize bufferSize = size;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanRHI::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    VkDevice device = VkContext::GetDevice();

    void* data;
    
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices, (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    VulkanRHI::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

    VulkanRHI::copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

XRE::VulkanVertexBuffer::~VulkanVertexBuffer()
{
    vkDestroyBuffer(VkContext::GetDevice(), m_vertexBuffer, nullptr);
    vkFreeMemory(VkContext::GetDevice(), m_vertexBufferMemory, nullptr);
}

void XRE::VulkanVertexBuffer::Bind() const
{
}

void XRE::VulkanVertexBuffer::Unbind() const
{
}

XRE::VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count)
{
    m_Count = count;
    VkDeviceSize bufferSize = sizeof(indices[0]) * count;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VulkanRHI::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    VkDevice device = VkContext::GetDevice();
    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices, (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    VulkanRHI::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

    VulkanRHI::copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

XRE::VulkanIndexBuffer::~VulkanIndexBuffer()
{
    vkDestroyBuffer(VkContext::GetDevice(), m_indexBuffer, nullptr);
    vkFreeMemory(VkContext::GetDevice(), m_indexBufferMemory, nullptr);

}

void XRE::VulkanIndexBuffer::Bind() const
{
}

void XRE::VulkanIndexBuffer::Unbind() const
{
}

XRE::VulkanUniformBuffer::VulkanUniformBuffer(VkDevice device, VkDeviceSize bufferSize)
{
    m_device = device;
    m_bufferSize = bufferSize;
    VulkanRHI::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffer, m_bufferMemory);
    vkMapMemory(device, m_bufferMemory, 0, bufferSize, 0, &m_bufferMapped);

    bufferInfo.buffer = GetBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);
}

XRE::VulkanUniformBuffer::~VulkanUniformBuffer()
{
    vkDestroyBuffer(m_device, m_uniformBuffer, nullptr);
    vkFreeMemory(m_device,m_bufferMemory, nullptr);
}

void XRE::VulkanUniformBuffer::WriteToBuffer(void* data)
{
    memcpy(m_bufferMapped, data, m_bufferSize);
}
