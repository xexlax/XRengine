#pragma once

#include "VulkanBuffers.h"
#include "VulkanImage.h"
namespace XRE {

    class VulkanDescriptorPool {


    public:
        VulkanDescriptorPool(uint32_t maxSets,
            const std::vector<VkDescriptorPoolSize>& poolSizes);


        ~VulkanDescriptorPool();

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();


        VkDescriptorPool descriptorPool;
    };

   
    class VulkanDescriptorWriter {

    public:

        VulkanDescriptorWriter();
        void createDescriptorSets();

        void writeBuffer(XRef<VulkanUniformBuffer> buffer);
        void writeImage(XRef<VulkanImage> image, uint32_t offset);
        void overwrite(int i);

        VkDescriptorSet* GetDescriptorSet(int offset) { return &descriptorSets[offset]; }

    private:
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkWriteDescriptorSet> writes;
    };
}  
