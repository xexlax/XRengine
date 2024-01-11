#include "OpenGLBuffers.h"
#include "pch.h"
#include "OpenGLFrameBuffer.h"
#include <glad\glad.h>
namespace XRE {

	namespace Utils {

		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static GLenum FBTextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
			case FramebufferTextureFormat::RGB8:       return GL_RGB8;
			case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			case FramebufferTextureFormat::RED_ONLY: return GL_R8;
			}

			XRE_CORE_ASSERT(false);
			return 0;
		}

	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		

		for (auto spec : m_Specification.Attachments)
		{
			if (spec != FramebufferTextureFormat::Depth)
				m_ColorAttachmentSpec.push_back(spec);
			else
				m_UseDepth = true;
		}
		



		Activate();
	}
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}
	void OpenGLFramebuffer::Activate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}


		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.Samples > 1;


		// Attachments
		if (m_ColorAttachmentSpec.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpec.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpec[i])
				{
				case FramebufferTextureFormat::RGBA8:
					
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RGB8:

					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGB8, GL_RGB, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RED_ONLY:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R8, GL_RED, m_Specification.Width, m_Specification.Height, i);
					break;
				}
				
					
			}
		}

		if (m_UseDepth)
		{
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
			
		}
		if (m_ColorAttachments.size() > 1)
		{
			XRE_CORE_ASSERT(m_ColorAttachments.size() <= 6);
			GLenum buffers[6] = {
				GL_COLOR_ATTACHMENT0, 
				GL_COLOR_ATTACHMENT1, 
				GL_COLOR_ATTACHMENT2, 
				GL_COLOR_ATTACHMENT3,
				GL_COLOR_ATTACHMENT4,
				GL_COLOR_ATTACHMENT5
			
			};
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, GetWidth(),GetHeight());
	}
	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Specification.Width = width;
		m_Specification.Height = height;
		Activate();
	}
	void OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y, void* p)
	{
		XRE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());


		GLint defaultFramebuffer = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFramebuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID );
		// textureId:  要读取的TextureID
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, GL_TEXTURE_2D,
			m_ColorAttachments[attachmentIndex], 0);

		//默认读取GL_COLOR_ATTACHMENT0，此处可以不设置
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, p);




		glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);

		
		

		
		
	

	}
	void OpenGLFramebuffer::ActiveDepth(int slot)
	{
		glActiveTexture(slot + GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);

	}
	void OpenGLFramebuffer::ActiveColor(int slot,int id)
	{
		
		glActiveTexture(slot+ GL_TEXTURE0 );
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[id]);

	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		XRE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

		auto& spec = m_ColorAttachmentSpec[attachmentIndex];
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
			Utils::FBTextureFormatToGL(spec), GL_INT, &value);
	}



}