
#include "VulkanVertexArray.h"
#include "VulkanBuffers.h"

#include <tiny_obj_loader.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
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

void XRE::VulkanVertexArray::loadModel(std::string path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::vector<VkVertex> vertices;
    std::vector<uint32_t> indices;



    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<VkVertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            VkVertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
    auto VBO = XMakeRef<VulkanVertexBuffer>((float*)&vertices[0], sizeof(VkVertex) * vertices.size());
    auto IBO = XMakeRef<VulkanIndexBuffer>((uint32_t*)&indices[0], indices.size());
    indexCount = indices.size();

    AddVertexBuffer(VBO);
    SetIndexBuffer(IBO);
}
