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
        /*  函数   */
        Model(char* path)
        {
            LoadModel(path);
        }
        void Draw(Ref<Shader> shader, glm::mat4 transform = glm::mat4(1.0f));
    private:
        /*  模型数据  */
        vector<Mesh> m_Meshes;
        vector<Ref<BaseMaterial>> m_Materials;
        string directory;
        /*  函数   */
        void LoadModel(string path,
            bool triangulate = true);
	};
}