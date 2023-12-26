
#pragma once

#include "xre/Renderer/Resources/Texture.h"
#include "VulkanImage.h"

namespace XRE {

	class VulkanTexture2D : public Texture2D
	{
	public:
		VulkanTexture2D(const std::string& path);
		virtual ~VulkanTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererId() override;

		void* GetDescriptor() override;

		virtual void Bind(uint32_t slot = 0) const override;

		XRef<VulkanImage> m_Image;


	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID = -1;

        void createTextureImage();

        void createTextureImageView();

		void createTextureSampler();

		


	};

}