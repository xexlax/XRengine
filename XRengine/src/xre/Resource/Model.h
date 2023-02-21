#pragma once
#include "pch.h"
#include "Mesh.h"
#include "Material.h"
#include "xre\Utils\Color.h"
using namespace std;
namespace XRE {
	

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
        vector<Ref<Material>> m_Materials;
        string directory;
        /*  ����   */
        void LoadModel(string path,
            bool triangulate = true);
	};
}