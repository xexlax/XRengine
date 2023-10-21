#include "D:/Study/bishe2023.1/dev/XRengine/XRengine/src/pch.h"
#include "VulkanVertexArray.h"

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
}
