#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

//What is device context?
// //A device context contains the circumstance or setting in which a device is used.
//A structure that defines a set of graphic objects and their associated attributes
//as well as the graphic modes that affect output.
//Graphic objects: pen for line drawing, brush for painting filling, bitmap for copying or scrolling, etc.


class SwapChain;
class VertexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;
class IndexBuffer;

class DeviceContext
{
public: 
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system);

	void clearRenderTargetColor(const SwapChainPtr swap_chain, float red, float green, float blue, float alpha);
	void setVertexBuffer(const VertexBufferPtr& vertex_buffer);
	void setIndexBuffer(const IndexBufferPtr& index_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);

	void setVertexShader(const VertexShaderPtr& vertex_shader);
	void setPixelShader(const PixelShaderPtr& pixel_shader);
	
	void setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer);
	void setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& buffer);

	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);

	void setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr& texture);
	void setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr& texture);


	~DeviceContext();
private:
	RenderSystem* m_system = nullptr;
	ID3D11DeviceContext* m_device_context;
private:
	friend class ConstantBuffer;
};

