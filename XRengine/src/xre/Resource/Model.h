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

        static XRef<Model> Create(const std::string& path);

        
       

        vector<XRef<Material>> m_defaultMaterials;
    private:

        void LoadModel(string path, bool triangulate = true);
        
        
        
        /*  ����   */
      

	};
}