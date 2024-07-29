#include "GraphicsEngineDX12.h"

GraphicsEngineDX12::GraphicsEngineDX12()
{
}

bool GraphicsEngineDX12::init()
{
	//DX11
	//D3D_DRIVER_TYPE driver_types;

	//DX12

	ID3D12Device* d3d12Device;
	auto result = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&d3d12Device);

	return true;
}

bool GraphicsEngineDX12::release()
{
	return true;
}

GraphicsEngineDX12::~GraphicsEngineDX12()
{
}
