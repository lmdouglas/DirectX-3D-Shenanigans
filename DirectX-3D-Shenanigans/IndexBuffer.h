#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class IndexBuffer
{
public:
	IndexBuffer(void* list_vertices, UINT size_vertex, RenderSystem* system);
	UINT getSizeIndexList();
	~IndexBuffer();
private:
	UINT m_size_list;
private:
	RenderSystem* m_system = nullptr;
	ID3D11Buffer* m_buffer;
private:
	friend class DeviceContext;
};

