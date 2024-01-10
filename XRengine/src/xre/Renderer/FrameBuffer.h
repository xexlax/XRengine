#pragma once
#include "pch.h"


namespace XRE {

	//新 Framebuffer 类的使用：新的FrameBuffer支持MRT（多目标渲染）
	//声明specification
	//添加texture specifaction（注意通常的渲染也需要添加深度buffer，以进行depth test）
	//调用Create
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		// Picking
		RED_INTEGER,
		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8,

		RGB8,
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		std::vector<FramebufferTextureFormat> Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};
	class Framebuffer
	{
		
		
	public:
		virtual ~Framebuffer() {};
		
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
		virtual void ReadPixel(uint32_t attachmentIndex, int x, int y, void* p) = 0;

		virtual uint32_t GetColorAttachment(uint32_t id=0)const = 0;
		virtual uint32_t GetDepthAttachment()const = 0;
		virtual uint32_t GetWidth()const = 0;
		virtual uint32_t GetHeight()const = 0;

		virtual void ActiveDepth(int slot)=0;
		virtual void ActiveColor(int slot,int id=0) = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		static XRef<Framebuffer> Create(const FramebufferSpecification& spec);
	protected:

	};


}