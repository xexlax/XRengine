#pragma once
#include "xre\Renderer\FrameBuffer.h"
namespace XRE {
	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(uint32_t width, uint32_t height);
		virtual ~OpenGLFramebuffer();

		void Activate();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachment()const { return m_ColorAttachment; };
		virtual uint32_t GetDepthAttachment()const { return m_DepthAttachment; };
		virtual uint32_t GetWidth()const { return m_Width; };
		virtual uint32_t GetHeight()const { return m_Height; };
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		
	};
}