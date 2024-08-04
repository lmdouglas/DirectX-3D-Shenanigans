#pragma once
#include <d3d11.h>

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
	DeviceContext(ID3D11DeviceContext* device_context);

	void clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	void setVertexBuffer(VertexBuffer* vertex_buffer);
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);


	void setViewportSize(UINT width, UINT height);

	void setVertexShader(VertexShader* vertex_shader);	
	void setPixelShader(PixelShader* pixel_shader);
	
	void setConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* buffer);
	void setConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* buffer);

	void setIndexBuffer(IndexBuffer* index_buffer);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);

	bool release();


	~DeviceContext();
private:
	ID3D11DeviceContext* m_device_context;
private:
	friend class ConstantBuffer;
};

