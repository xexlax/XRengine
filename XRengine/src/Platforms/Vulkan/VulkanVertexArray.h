#pragma once

#include "xre/Renderer/Resources/VertexArrays.h"
#include "vulkan\vulkan.h"
#include "VulkanDescriptor.h"
#include "VulkanTexture.h"
namespace XRE{

	//Vulkan doesn't really have concept of VAO
	//Just Replace as Model
	class VulkanVertexArray : public VertexArray
	{
	public:
		VulkanVertexArray();
		virtual ~VulkanVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

		void Bind(VkCommandBuffer commandBuffer);

		

		void Draw(VkCommandBuffer commandBuffer);

		VkDescriptorSet* GetDescriptorSet(int offset) {
			return descriptorWriter->GetDescriptorSet(offset);
		}

		void UpdateDescriptorSets(const std::vector<XRef<Texture2D>>& textures);

		void createDescriptorSets(const std::vector<XRef<Texture2D>>& textures);


	private:
		uint32_t indexCount;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		XRef<VulkanDescriptorWriter> descriptorWriter = nullptr;
		

	};

	struct VkVertex {
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;


		bool operator==(const VkVertex& other) const {
			return pos == other.pos && color == other.color && texCoord == other.texCoord;
		}
	};

}