#include "pch.h"
#include "Model.h"
#include "xre\Core\Macros.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

//#define MODEL_DEBUG

namespace XRE {

	float ShininessToRoughness(float Ypoint)
	{
		float a = -1;
		float b = 2;

		float c;
		c = (Ypoint / 100) - 1;

		float D;
		D = b * b - (4 * a * c);

		float x1;
		x1 = (-b + sqrt(D)) / (2 * a);

		return x1;
	}

	struct equal_idx
	{
		bool operator()(const tinyobj::index_t& a, const tinyobj::index_t& b) const
		{
			return a.vertex_index == b.vertex_index
				&& a.texcoord_index == b.texcoord_index
				&& a.normal_index == b.normal_index;
		}
	};

	struct hash_idx
	{
		size_t operator()(const tinyobj::index_t& a) const
		{
			return ((a.vertex_index
				^ a.texcoord_index << 1) >> 1)
				^ (a.normal_index << 1);
		}
	};
	
	
	void Model::Draw(Ref<Shader> shader, glm::mat4 transform)
	{
		for (auto mesh : m_Meshes) {
			mesh.Draw(shader);
		}
	}
	void Model::LoadModel(string path,bool triangulate)
	{
		std::string bps;
		size_t pos = path.find_last_of('/');
		if (pos != std::string::npos) {
				bps = path.substr(0, pos);
		}
		
		
		XRE_CORE_TRACE("Loading Model From {0},{1}", path, bps);
		
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		
		std::string warn;
		std::string err;
		
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(),
			bps.c_str(), triangulate);
		
		if (!warn.empty()) {
			XRE_CORE_WARN("WARN: {0}", warn);
		}

		if (!err.empty()) {
			XRE_CORE_ERROR("ERROR: {0}", err);
		}

		if (!ret) {
			XRE_CORE_ERROR("Failed to load/parse .obj. ");
		}

#ifdef MODEL_DEBUG
		XRE_CORE_INFO("Vertex Count:{0}", attrib.vertices.size() / 3);
		XRE_CORE_INFO("Normal Count:{0}", attrib.normals.size() / 3);
		XRE_CORE_INFO("UV Count:{0}", attrib.texcoords.size() / 2);
		XRE_CORE_INFO("Mesh Count:{0}", shapes.size());
		XRE_CORE_INFO("Texture Count:{0}", materials.size());
#endif // MODEL_DEBUG

		

		///1. 获取各种材质和纹理
		{
			for (int i = 0; i < materials.size(); i++) {
				Ref<BaseMaterial> m = make_shared<BaseMaterial>();
				tinyobj::material_t tm = materials[i];
				
				string name = tm.name;
				if (name.size()) {
					m->name = name;
				}
				m->ambient.r = tm.ambient[0];
				m->ambient.g = tm.ambient[1];
				m->ambient.b = tm.ambient[2];

				m->diffuse.r = tm.diffuse[0];
				m->diffuse.g = tm.diffuse[1];
				m->diffuse.b = tm.diffuse[2];

				m->specular.r = tm.specular[0];
				m->specular.g = tm.specular[1];
				m->specular.b = tm.specular[2];

				m->transmittance.r = tm.transmittance[0];
				m->transmittance.g = tm.transmittance[1];
				m->transmittance.b = tm.transmittance[2];

				m->emission.r = tm.emission[0];
				m->emission.g = tm.emission[1];
				m->emission.b = tm.emission[2];

				m->shininess = tm.shininess;
				m->ior = tm.ior;
				m->dissolve = tm.dissolve;
				m->illum = tm.illum;
				m->pad0 = tm.pad0;


				m_Materials.push_back(m);
			}

		}


		


		/// 2.顶点数据
		{
			// For each shape 遍历每一个部分
			for (size_t i = 0; i < shapes.size(); i++) {

				
				
#ifdef MODEL_DEBUG

				
				XRE_CORE_INFO("Shape{0} :{1}, Size:{2},Faces:{3}", i, shapes[i].name,
					shapes[i].mesh.indices.size(), shapes[i].mesh.num_face_vertices.size());
			
#endif // MODEL_DEBUG

				std::unordered_map<tinyobj::index_t,uint32_t,hash_idx, equal_idx > uniqueVertices = {};
				// 开辟空间
				vector<Vertex> vertices;
				vector<uint32_t> indices;
				size_t index_offset = 0;

				for (const auto& index: shapes[i].mesh.indices) {
					Vertex vertex={};
					vertex.Position = {
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					};

					vertex.Normal = {
						attrib.vertices[3 * index.normal_index + 0],
						attrib.vertices[3 * index.normal_index + 1],
						attrib.vertices[3 * index.normal_index + 2]
					};

					vertex.TexCoords = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
					};

					if (uniqueVertices.count(index) == 0) {
						uniqueVertices[index] = static_cast<uint32_t>(vertices.size());
						vertices.push_back(vertex);
					}
					indices.push_back(uniqueVertices[index]);


				}

					
				
				Mesh cur_mesh(vertices,indices);
				m_Meshes.push_back(cur_mesh);
			}

			XRE_CORE_INFO("已完成读取模型");
		}
	}
	

}