
#include "VulkanMaterial.h"

#include "VulkanSwapChain.h"
#include "VkContext.h"
#include "VulkanTexture.h"
#include "VulkanStructs.h"
#include "xre\Resource\ResourceManager.h"

XRE::VulkanMaterial::VulkanMaterial(std::string path) :
	Material(path) {
    
	MaterialUBOs.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
	for (size_t i = 0; i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
		MaterialUBOs[i] = XMakeRef<VulkanUniformBuffer>(VkContext::GetDevice(), sizeof(MaterialUBO));
	}

    
}

void XRE::VulkanMaterial::WriteDS()
{

   

    descriptorWriter = XMakeRef<VulkanDescriptorWriter>(VkContext::GetInstance()->modelPipeline);
    for (int i = 0;i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;i++) {
        descriptorWriter->writeBuffer(VkContext::GetInstance()->modelPipeline->GlobalUBOs[i]);
        descriptorWriter->writeBuffer(VkContext::GetInstance()->modelPipeline->LightingUBOs[i]);
        descriptorWriter->writeBuffer(MaterialUBOs[i]);
        if(diffuseTex.m_Tex)
            descriptorWriter->writeImage(std::dynamic_pointer_cast<VulkanTexture2D>(diffuseTex.m_Tex)->m_Image);
        else
            descriptorWriter->writeImage(std::dynamic_pointer_cast<VulkanTexture2D>(ResourceManager::GetDefaultTex2D())->m_Image);
        if (specularTex.m_Tex)
            descriptorWriter->writeImage(std::dynamic_pointer_cast<VulkanTexture2D>(specularTex.m_Tex)->m_Image);
        else
            descriptorWriter->writeImage(std::dynamic_pointer_cast<VulkanTexture2D>(ResourceManager::GetDefaultTex2D())->m_Image);
        if (bumpTex.m_Tex)
            descriptorWriter->writeImage(std::dynamic_pointer_cast<VulkanTexture2D>(bumpTex.m_Tex)->m_Image);
        else
            descriptorWriter->writeImage(std::dynamic_pointer_cast<VulkanTexture2D>(ResourceManager::GetDefaultTex2D())->m_Image);
        descriptorWriter->overwrite(i);

    }
    
}

void XRE::VulkanMaterial::Bind()
{
    if (!setup) {
        WriteDS();
        setup = true;
    }
    
        MaterialUBO mubo;
        mubo.useAlbedo = diffuseTex.m_enable;
        mubo.useNormalMap = bumpTex.m_enable;
        mubo.useSpecMap = specularTex.m_enable;
        mubo.metallic = metallic;
        mubo.roughness = roughness;
        
        MaterialUBOs[VkContext::GetCurrentFrame()]->WriteToBuffer(&mubo);
    
}
