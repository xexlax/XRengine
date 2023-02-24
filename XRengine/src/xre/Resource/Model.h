#pragma once
#include "pch.h"
#include "Mesh.h"
#include "Material.h"
#include "xre\Utils\Color.h"
using namespace std;
namespace XRE {
	

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