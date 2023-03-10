#include "pch.h"
#include "VertexArrays.h"

#include "xre\Renderer\Renderer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"

namespace XRE {

	XRef<VertexArray>  VertexArray::CubeVA= nullptr, VertexArray::CircleVA = nullptr , VertexArray::SquareVA = nullptr , VertexArray::CapsuleVA = nullptr;
	float VertexArray::Capsule_r=0, VertexArray::Capsule_l=0;
	XRef<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    XRE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}

		XRE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	XRef<VertexArray> VertexArray::GetCubeVA()
	{
		if (!CubeVA) 
		{
			glm::vec3 points[8];
			points[0] = {  1,  1,  1 };
			points[1] = {  1,  1, -1 };
			points[2] = {  1, -1,  1 };
			points[3] = {  1, -1, -1 };
			points[4] = { -1,  1,  1 };
			points[5] = { -1,  1, -1 };
			points[6] = { -1, -1,  1 };
			points[7] = { -1, -1, -1 };

			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
			};

			CubeVA = VertexArray::Create();


			//2.1 setup VBO
			XRef<VertexBuffer> vertexBuffer;;
			vertexBuffer.reset(VertexBuffer::Create((float*)&points[0], 8 * layout.GetStride()));
			vertexBuffer->SetLayout(layout);
			CubeVA->AddVertexBuffer(vertexBuffer);

			uint32_t indices[16] = {
				0,1,3,2,0,4,5,1,3,7,5,4,6,7,6,2
			};

			XRef<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, 16));
			CubeVA->SetIndexBuffer(indexBuffer);

			
		}
		CubeVA->Bind();
		return CubeVA;
	}

	XRef<VertexArray> VertexArray::GetCircleVA()
	{
		if (!CircleVA)
		{
			std::vector<glm::vec3> points;
			int num = 20;
			float pi = 3.1415926f;

			uint32_t indices[20];
			for (int i = 0;i < num;i++) {
				float t = i / 20.0f *2*pi;
				glm::vec3 p(cos(t), 0, sin(t));
				points.push_back(p);
				indices[i] = i;
			}

			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
			};

			CircleVA = VertexArray::Create();

			//2.1 setup VBO
			XRef<VertexBuffer> vertexBuffer;;
			vertexBuffer.reset(VertexBuffer::Create((float*)&points[0], num * layout.GetStride()));
			vertexBuffer->SetLayout(layout);
			CircleVA->AddVertexBuffer(vertexBuffer);

			

			XRef<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, num));
			CircleVA->SetIndexBuffer(indexBuffer);



			

		}
		CircleVA->Bind();
		return CircleVA;
	}

	XRef<VertexArray> VertexArray::GetSquareVA()
	{
		return XRef<VertexArray>();
	}

	XRef<VertexArray> VertexArray::GetCapsuleVA(float r, float l)
	{

		if (r != Capsule_r || l != Capsule_l) {
			Capsule_r = r;
			Capsule_l = l;
			std::vector<glm::vec3> points;
			int num = 21;
			float pi = 3.1415926f;

			uint32_t indices[21];
			for (int i = 0;i < 10;i++) {
				float t = i / 20.0f * 2 * pi;
				glm::vec3 p(cos(t)*r,l+ sin(t) * r,  0);
				points.push_back(p);
				indices[i] = i;
			}
				glm::vec3 p(cos(pi) * r, sin(pi) * r,0);
				points.push_back(p);
				indices[10] = 10;

			for (int i = 10;i < 20;i++) {
				float t = i / 20.0f * 2 * pi;
				glm::vec3 p(cos(t) * r,-l+ sin(t) * r,0);
				points.push_back(p);
				indices[i+1] = i+1;
			}

			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
			};

			CapsuleVA = VertexArray::Create();

			//2.1 setup VBO
			XRef<VertexBuffer> vertexBuffer;;
			vertexBuffer.reset(VertexBuffer::Create((float*)&points[0], num * layout.GetStride()));
			vertexBuffer->SetLayout(layout);
			CapsuleVA->AddVertexBuffer(vertexBuffer);



			XRef<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, num));
			CapsuleVA->SetIndexBuffer(indexBuffer);
		}
			
		CapsuleVA->Bind();
		return CapsuleVA;
	}
	

}