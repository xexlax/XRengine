#pragma once
#include "pch.h"
#include "Mesh.h"
#include "Material.h"
using namespace std;
namespace XRE {
	
    //һ��Model������Mesh��ɣ�ÿ��Meshӵ��Ψһ��Material
	class Model {
    public:

        Model(std::string path):
            m_Path(path)
        {
            LoadModel(path);

        }
     
        vector<Mesh> m_Meshes;
        string getPath()const { return m_Path; };
        std::string m_Path;

        static Ref<Model> Create(const std::string& path);
    private:
        
        vector<Ref<Material>> m_Materials;
        /*  ����   */
        void LoadModel(string path,
            bool triangulate = true);


	};
}