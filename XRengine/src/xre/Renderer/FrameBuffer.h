#pragma once
#include "pch.h"


namespace XRE {

	class Framebuffer
	{
		
		
	public:
		Framebuffer(uint32_t width, uint32_t height);
		virtual ~Framebuffer() {};
		
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachment()const = 0;
		virtual uint32_t GetDepthAttachment()const = 0;
		virtual uint32_t GetWidth()const = 0;
		virtual uint32_t GetHeight()const = 0;
		static Ref<Framebuffer> Create(uint32_t width, uint32_t height);
	protected:
		uint32_t m_Width, m_Height;
	};


}