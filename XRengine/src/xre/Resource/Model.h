#pragma once
#include "pch.h"
#include "Mesh.h"
#include "Material.h"
using namespace std;
namespace XRE {
	
    //һ��Model������Mesh��ɣ�ÿ��Meshӵ��Ψһ��Material
	class Model {
    public:
        /*  ����   */
        Model(std::string path)
        {
            LoadModel(path);
        }
     
        vector<Mesh> m_Meshes;
    private:
        /*  ģ������  */
        
        vector<Ref<Material>> m_Materials;
        string directory;
        /*  ����   */
        void LoadModel(string path,
            bool triangulate = true);
	};
}