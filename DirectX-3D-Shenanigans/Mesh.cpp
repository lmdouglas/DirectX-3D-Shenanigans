#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <locale> //For wstring_convert
#include <codecvt>

#include "GraphicsEngine.h"
#include "VertexMesh.h"

Mesh::Mesh(const wchar_t* full_path): Resource(full_path)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warnings;
	std::string errors;


	unsigned int size_needed = WideCharToMultiByte(CP_UTF8, 0, &full_path[0], wcslen(full_path), NULL, 0, NULL, NULL);
	std::string inputfile(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &full_path[0], wcslen(full_path), &inputfile[0], size_needed, NULL, NULL);
	//std::string inputfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);

	//Add MTL file
	std::size_t found = inputfile.find_last_of('\\');
	std::string directory = inputfile.substr(0, found);
	//std::string directory = ".";
	//WideCharToMultiByte(CP_UTF8, 0, &full_path[0], wcslen(full_path), &directory[0], size_needed, NULL, NULL);

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warnings, &errors, inputfile.c_str(), directory.c_str());

	if (!errors.empty())
	{
		throw std::exception("Mesh not created successfully. Errors not empty.");
	}

	if (!res) throw std::exception("Mesh not created successfully. Res Error.");

	if (shapes.size() > 1) throw std::exception("Multiple shapes in mesh not yet supported.");

	//Vector of vertex meshes
	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	try {
		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;
			list_vertices.reserve(shapes[s].mesh.indices.size());
			list_indices.reserve(shapes[s].mesh.indices.size());

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					//Get vertex coordinates
					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0]; // Multiply by 3 as list holds X,Y,Z sequentially
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					//Get texture coordinates
					tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

					//Get normals - for light
					tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0]; // Multiply by 3 as list holds X,Y,Z sequentially
					tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];


					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz));
					list_vertices.push_back(vertex);
					list_indices.push_back((unsigned int)index_offset + v);

				}

				index_offset += num_face_verts;

			}
		}
	}
	catch (...)
	{
		bool test = true;
	}

	//Needed to create a new shader at this point purely for this purpose
	size_t size_shader = 0;
	void* shader_byte_code = nullptr;

	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh), (UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);

	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_index_buffer;
}
