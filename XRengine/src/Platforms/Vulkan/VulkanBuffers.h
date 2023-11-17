#pragma once


#include "xre/Renderer/Resources/Buffers.h"
#include "vulkan\vulkan.h"
namespace XRE {

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(float* vertices, uint32_t size);
		virtual ~VulkanVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
	private:
		
		
		BufferLayout m_Layout;
	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~VulkanIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }
		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory;
	private:
		
		uint32_t m_Count;
	};


	class VulkanUniformBuffer
	{
	public:
		VulkanUniformBuffer(VkDevice device, VkDeviceSize bufferSize);
		~VulkanUniformBuffer();
		VkBuffer GetBuffer() { return m_uniformBuffer; }
		void WriteToBuffer(void* data);

	private:
		VkDevice m_device;
		VkDeviceSize m_bufferSize;
		VkBuffer m_uniformBuffer;
		VkDeviceMemory m_bufferMemory;
		void* m_bufferMapped;
	};
}