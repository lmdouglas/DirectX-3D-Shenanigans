#pragma once
#include "Vector2D.h"
#include "Vector3D.h"

class VertexMesh
{
public:
	VertexMesh() :m_position(), m_texcoord()
	{
	}

	VertexMesh(Vector3D position, Vector2D texcoord) : m_position(position), m_texcoord(texcoord)
	{
	}

	VertexMesh(const VertexMesh& vertexmesh) : m_position(vertexmesh.m_position), m_texcoord(vertexmesh.m_texcoord)
	{
	}
		
	~VertexMesh()
	{
	}
public:
	Vector3D m_position;
	Vector2D m_texcoord;
};