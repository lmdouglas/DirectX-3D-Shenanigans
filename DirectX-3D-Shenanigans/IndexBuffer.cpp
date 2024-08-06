#include "IndexBuffer.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include <exception>


IndexBuffer::IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system) :m_buffer(0), m_system(system)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = 4 * size_list; //each element of index buffer is 4 bytes
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_indices;

	m_size_list = size_list;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("IndexBuffer");
	}

}


UINT IndexBuffer::getSizeIndexList()
{
	return this->m_size_list;
}


IndexBuffer::~IndexBuffer()
{
	m_buffer->Release();
}
