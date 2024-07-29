#pragma once
#include <d3d12.h>

class GraphicsEngineDX12
{
public:
	GraphicsEngineDX12();
	//Initialise the GraphicsEngine and DirectX 11 Device
	bool init();
	//Release all the resources loaded
	bool release();
	~GraphicsEngineDX12();
};

