#include "pch.h"
#include "Model.h"
#include "xre\Renderer\Resources\Texture.h"
#include "xre\Resource\ResourceManager.h"
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
	

	XRef<Model> Model::Create(const std::string& path)
	{
		return std::make_shared<Model>(path);
	}

	

	void Model::LoadModel(string path,bool triangulate)
	{
		std::string basepath;


		size_t pos = path.find_last_of('\\');
		if (pos != std::string::npos) {
			basepath = path.substr(0, pos);
		}
		
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		
		std::string warn;
		std::string err;
		
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(),
			basepath.c_str(), triangulate);
		
		if (!warn.empty()) {
			XRE_CORE_WARN("WARN: {0}", warn);
		}

		if (!err.empty()) {
			XRE_CORE_ERROR("ERROR: {0}", err);
		}

		if (!ret) {
			XRE_CORE_ERROR("����ʧ�� .obj. ");
		}

#ifdef MODEL_DEBUG
		XRE_CORE_INFO("Vertex Count:{0}", attrib.vertices.size() / 3);
		XRE_CORE_INFO("Normal Count:{0}", attrib.normals.size() / 3);
		XRE_CORE_INFO("UV Count:{0}", attrib.texcoords.size() / 2);
		XRE_CORE_INFO("Mesh Count:{0}", shapes.size());
		XRE_CORE_INFO("Texture Count:{0}", materials.size());
#endif // MODEL_DEBUG

		

		{
			for (int i = 0; i < materials.size(); i++) {
				XRef<Material> m = make_shared<Material>();
			
				tinyobj::material_t tm = materials[i];
				
				string name = tm.name;
				m->SetPath(basepath + "\\" + name + ".mat");
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
				m->roughness = ShininessToRoughness(tm.shininess);
				m->ior = tm.ior;
				m->dissolve = tm.dissolve;
				m->illum = tm.illum;
				m->pad0 = tm.pad0;
				

				if (tm.diffuse_texname!="") {
					std::string  diffuse_path = basepath +'/' + tm.diffuse_texname;
					m->diffuseTex = MaterialTex(true, tm.diffuse_texname, diffuse_path);
					//XRE_CORE_INFO("diffuse_Tex {0}", diffuse_path);
				}

				if (tm.specular_texname != "") {
					std::string  specular_path = basepath + '/' + tm.specular_texname;
					m->specularTex = MaterialTex(true, tm.specular_texname, specular_path);
					//XRE_CORE_INFO("specular_Tex {0}", specular_path);
				}

				if (tm.bump_texname != "") {
					std::string  bump_path = basepath + '/' + tm.bump_texname;
					m->bumpTex = MaterialTex(true, tm.bump_texname, bump_path);
					//XRE_CORE_INFO("specular_Tex {0}", specular_path);
				}
				m->LoadAllTex();
				m_defaultMaterials.push_back(m);
			}

		}


		


		/// 2.��������
		{

			
			
			// For each shape 
			for (size_t i = 0; i < shapes.size(); i++) {
				
				std::unordered_map<tinyobj::index_t, uint32_t, hash_idx, equal_idx > uniqueVertices = {};
	
				vector<Vertex> vertices;
				vector<uint32_t> indices;
				
#ifdef MODEL_DEBUG

				
				XRE_CORE_INFO("Shape{0} :{1}, Size:{2},Faces:{3}", i, shapes[i].name,
					shapes[i].mesh.indices.size(), shapes[i].mesh.num_face_vertices.size());
			
#endif // MODEL_DEBUG

				glm::vec3 HigherBorder;
				glm::vec3 LowerBorder;
				
				size_t index_offset = 0;
				for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
					size_t fnum = shapes[i].mesh.num_face_vertices[f];
					tinyobj::index_t index;
					Vertex vertex[3];
					for (size_t v = 0; v < fnum; v++) {
						index = shapes[i].mesh.indices[index_offset + v];
						
						// v
						if(index.vertex_index>=0)
						vertex[v].Position = {
							attrib.vertices[3 * index.vertex_index + 0],
							attrib.vertices[3 * index.vertex_index + 1],
							attrib.vertices[3 * index.vertex_index + 2]
						};


						// Update HigherBorder and LowerBorder with the maximum and minimum position values
						if (v == 0 && f == 0) {
						    HigherBorder = vertex[v].Position;
						    LowerBorder = vertex[v].Position;
						} else {
						    HigherBorder.x = glm::max(HigherBorder.x, vertex[v].Position.x);
						    HigherBorder.y = glm::max(HigherBorder.y, vertex[v].Position.y);
						    HigherBorder.z = glm::max(HigherBorder.z, vertex[v].Position.z);
						    LowerBorder.x = glm::min(LowerBorder.x, vertex[v].Position.x);
						    LowerBorder.y = glm::min(LowerBorder.y, vertex[v].Position.y);
						    LowerBorder.z = glm::min(LowerBorder.z, vertex[v].Position.z);
						}
						
						if (index.normal_index >= 0)
						vertex[v].Normal = {
							attrib.normals[3 * index.normal_index + 0],
							attrib.normals[3 * index.normal_index + 1],
							attrib.normals[3 * index.normal_index + 2]
						};
						if (index.texcoord_index >= 0)
						vertex[v].TexCoords = {
							attrib.texcoords[2 * index.texcoord_index + 0],
							attrib.texcoords[2 * index.texcoord_index + 1]
						};
						

						//indices.push_back(vertices.size());
						//vertices.push_back(vertex);
					}

					glm::vec3 tangent{ 1, 0, 0 };
					{
						glm::vec3 edge1 = vertex[1].Position - vertex[0].Position;
						glm::vec3 edge2 = vertex[2].Position - vertex[1].Position;
						glm::vec2 deltaUV1 = vertex[1].TexCoords - vertex[0].TexCoords;
						glm::vec2 deltaUV2 = vertex[2].TexCoords - vertex[1].TexCoords;

						auto divide = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
						if (divide >= 0.0f && divide < 0.000001f)
							divide = 0.000001f;
						else if (divide < 0.0f && divide > -0.000001f)
							divide = -0.000001f;

						float df = 1.0f / divide;
						tangent.x = df * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
						tangent.y = df * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
						tangent.z = df * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
						tangent = glm::normalize(tangent);
					}
					
					for (size_t v = 0; v < fnum; v++) {
						vertex[v].Tangent = tangent;
						index = shapes[i].mesh.indices[index_offset + v];
						if (uniqueVertices.count(index) == 0) {
							uniqueVertices[index] = static_cast<uint32_t>(vertices.size());
							vertices.push_back(vertex[v]);
						}
						indices.push_back(uniqueVertices[index]);
					}					

					// ƫ��
					index_offset += fnum;

				}
				Mesh cur_mesh(vertices, indices);
				auto& m = shapes[i].mesh.material_ids;
				if(m[0]==-1) cur_mesh.MatID = 0;
				else
				cur_mesh.MatID = m[0];
				//XRE_CORE_INFO("Mesh{0},{1}vertexes,{2}indexes �Ѽ���",i, vertices.size(), indices.size());
				cur_mesh.m_AABB.HigherBorder = HigherBorder;
				cur_mesh.m_AABB.LowerBorder = LowerBorder;
				m_Meshes.push_back(cur_mesh);
				
			}
			
		}
	}
	

}