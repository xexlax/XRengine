#pragma once

#include "VulkanBuffers.h"
#include "VulkanImage.h"
#include "VulkanPipeline.h"
namespace XRE {

    class VulkanDescriptorPool {


    public:
        VulkanDescriptorPool(uint32_t maxSets,
             VkDescriptorPoolSize* poolSizes);


        ~VulkanDescriptorPool();

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();


        VkDescriptorPool descriptorPool;
    };

   
    class VulkanDescriptorWriter {

    public:
        
        VulkanDescriptorWriter(XRef<VulkanPipeline> pipeline);
        void createDescriptorSets(XRef<VulkanPipeline> pipeline);

        void writeBuffer(XRef<VulkanUniformBuffer> buffer);
        void writeImage(XRef<VulkanImage> image);
        void overwrite(int i);

        VkDescriptorSet* GetDescriptorSet(int offset) { return &descriptorSets[offset]; }

    private:
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkWriteDescriptorSet> writes;
    };
}  
