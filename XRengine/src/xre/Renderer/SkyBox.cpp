#include "pch.h"
#include "SkyBox.h"

#include "xre\Resource\ResourceManager.h"
#include "Platforms\OpenGL\OpenGLCubeMap.h"

namespace XRE {

	SkyBox::SkyBox()
	{
        m_SkyBoxShader = ResourceManager::GetShader("../Assets/shaders/skybox.glsl");
        
       /* std::vector<std::string> faces
        {
            "../Assets/textures/Skybox/right.jpg",
            "../Assets/textures/Skybox/left.jpg",
            "../Assets/textures/Skybox/top.jpg",
            "../Assets/textures/Skybox/bottom.jpg",
            "../Assets/textures/Skybox/front.jpg",
            "../Assets/textures/Skybox/back.jpg"
        };*/

        std::vector<std::string> faces
        {
            "../Assets/textures/Skybox1/px.png",
            "../Assets/textures/Skybox1/nx.png",
            "../Assets/textures/Skybox1/py.png",
            "../Assets/textures/Skybox1/ny.png",
            "../Assets/textures/Skybox1/pz.png",
            "../Assets/textures/Skybox1/nz.png"
        };

        CreateCubeMap(m_TextureID, faces);
        
        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
        };

        m_VertexArray = VertexArray::Create();

        XRef<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(skyboxVertices,sizeof(skyboxVertices)));
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);
        

	}
	SkyBox::~SkyBox()
	{
       
	}
}