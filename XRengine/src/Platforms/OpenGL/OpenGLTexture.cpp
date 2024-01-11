#include "pch.h"
#include "OpenGLTexture.h"

#ifdef  XRE_RENDERER_OPENGL

#endif //  XRE_RENDERER_OPENGL


#include <glad/glad.h>
#include "xre\Utils\image.h"

namespace XRE {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		m_name = path;
		int width, height, channels;
		unsigned char* data= load_image( path, &width, &height, &channels, 0, true);
		
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		XRE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1,internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height,dataFormat , GL_UNSIGNED_BYTE, data);

		image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(char* data, int width, int height)
	{

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		

		XRE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_FLOAT, data);

		//image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(slot);
		glBindTextureUnit(slot, m_RendererID);
	}
}