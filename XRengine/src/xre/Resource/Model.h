#pragma once
#include "pch.h"
#include "Mesh.h"
#include "xre\Utils\Color.h"
using namespace std;
namespace XRE {
	struct BaseMaterial
	{
		std::string name;
		glm::vec3 ambient,diffuse,specular,transmittance,emission;
		float shininess, ior, dissolve;
		int illum, pad0;

		Ref<Texture> ambientTex, diffuseTex, specularTex, specularHighlightTex,
            bumpTex, displacementTex, alphaTex;
		
	};

	class Model {
    public:
        /*  ����   */
        Model(char* path)
        {
            LoadModel(path);
        }
        void Draw(Ref<Shader> shader, glm::mat4 transform = glm::mat4(1.0f));
    private:
        /*  ģ������  */
        vector<Mesh> m_Meshes;
        vector<Ref<BaseMaterial>> m_Materials;
        string directory;
        /*  ����   */
        void LoadModel(string path,
            bool triangulate = true);
	};
}