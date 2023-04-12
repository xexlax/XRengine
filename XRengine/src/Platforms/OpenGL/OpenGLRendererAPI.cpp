#include "pch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace XRE {

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::CullFace(bool b)
	{
		if (b) glCullFace(GL_FRONT);
		else glCullFace(GL_BACK);
	}

	void OpenGLRendererAPI::SetDepthTest(bool b)
	{
		if(b) glDepthFunc(GL_LESS);
		else glDepthFunc(GL_ALWAYS);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawIndexedLine(const std::shared_ptr<VertexArray>& vertexArray)
	{
		/*glPolygonMode(GL_FRONT, GL_LINE);
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glPolygonMode(GL_FRONT, GL_FILL);*/

		glDrawElements(GL_LINE_STRIP, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

	}	

	void OpenGLRendererAPI::DrawIndexedLineLoop(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_LINE_LOOP, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawSkyIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t cubemapTexture)
	{
		//glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
	}

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glShadeModel(GL_SMOOTH);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}



}