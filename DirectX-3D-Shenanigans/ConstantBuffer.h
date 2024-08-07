#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system);
	void update(DeviceContextPtr context, void* buffer);
	~ConstantBuffer();
private:
	RenderSystem* m_system = nullptr;
	ID3D11Buffer* m_buffer = nullptr;
private:
	friend class DeviceContext;
};

