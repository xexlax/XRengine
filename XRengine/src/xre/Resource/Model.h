#pragma once
#include "pch.h"
#include "Mesh.h"
#include "Material.h"
using namespace std;
namespace XRE {
	
    //一个Model由数个Mesh组成，每个Mesh拥有唯一的Material
	class Model {
    public:
        /*  函数   */
        Model(std::string path)
        {
            LoadModel(path);
        }
     
        vector<Mesh> m_Meshes;
    private:
        /*  模型数据  */
        
        vector<Ref<Material>> m_Materials;
        string directory;
        /*  函数   */
        void LoadModel(string path,
            bool triangulate = true);
	};
}