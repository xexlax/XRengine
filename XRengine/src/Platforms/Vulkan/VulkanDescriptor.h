#pragma once

#include "VkContext.h"

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

    class VulkanDescriptorSetLayout {

    public:
        class Builder {
        public:
            
            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<VulkanDescriptorSetLayout> build() const;

        private:
     
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        VulkanDescriptorSetLayout(
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);

        ~VulkanDescriptorSetLayout();
        VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
        VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout&) = delete;

        

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
   
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class VulkanDescriptorWriter;
    };

   

    class VulkanDescriptorWriter {
    public:

        VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool);

        void writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        void writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        VulkanDescriptorSetLayout& setLayout;
        VulkanDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}  
