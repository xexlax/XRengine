#pragma once
#include "xre\Renderer\FrameBuffer.h"
namespace XRE {
	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferSpecification & spec);
		virtual ~OpenGLFramebuffer();

		void Activate();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void ReadPixel(uint32_t attachmentIndex, int x, int y, void* p) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;
		virtual uint32_t GetColorAttachment(uint32_t index = 0) const override 
		{ XRE_CORE_ASSERT(index < m_ColorAttachments.size()); 
		  return m_ColorAttachments[index]; }
		virtual uint32_t GetDepthAttachment()const { return m_DepthAttachment; };
		virtual uint32_t GetWidth()const { return m_Specification.Width; };
		virtual uint32_t GetHeight()const { return m_Specification.Height; };

		virtual void ActiveColor(int slot,int id);
		virtual void ActiveDepth(int slot);

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		FramebufferSpecification m_Specification;

		bool m_UseDepth=false;
		std::vector<FramebufferTextureFormat> m_ColorAttachmentSpec;
		
		uint32_t m_RendererID = 0;
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
		
	};
}