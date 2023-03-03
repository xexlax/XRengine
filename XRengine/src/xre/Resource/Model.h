#pragma once
#include "pch.h"
#include "Mesh.h"
#include "Material.h"
using namespace std;
namespace XRE {
	
    //一个Model由数个Mesh组成，每个Mesh拥有唯一的Material
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
        /*  函数   */
        void LoadModel(string path,
            bool triangulate = true);


	};
}