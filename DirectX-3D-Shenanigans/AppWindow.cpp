#include "AppWindow.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
};

AppWindow::AppWindow()
{

}

AppWindow::~AppWindow()
{

}

void AppWindow::onCreate()
{
	window::onCreate();
	GraphicsEngine::get()->init(); 
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();

	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	//Triangle vertices
	vertex list[] = {
		//X-Y-Z Triangle 1 (For TriangleList)
		//{-0.5f,-0.5f,0.0f}, //POS1
		//{-0.5f, 0.5f, 0.0f}, //POS2
		//{0.5f, 0.5f, 0.0f}, //POS3

		//X-Y-Z Triangle 2 (For TriangleList)
		//{0.5f,0.5f,0.0f}, //POS1
		//{0.5f, -0.5f, 0.0f}, //POS2
		//{-0.5f, -0.5f, 0.0f} //POS3


		//Two triangles (For TriangleStrip)
		{-0.5f,-0.5f,0.0f}, //bottom left
		{-0.5f,0.5f,0.0f}, //top left
		{0.5f,-0.5f,0.0f}, //bottom right
		{0.5f,0.5f,0.0f} //top right


		//X-Y-Z Original triangle
		//{-0.5f,-0.5f,0.0f}, //POS1
		//{0.0f, 0.5f, 0.0f}, //POS2
		//{0.5f, -0.5f, 0.0f} //POS3
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs=GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	//GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);

	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	window::onUpdate();

	//Clear the render target
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0,0.3f,0.4f,1);

	//Set viewport of render target in which we have to draw
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	//Set the default shader in the graphics pipeline to be able to draw
	GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);

	//Set the vertices of the triangle to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	//Draw the triangles from list
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0);

	//Altneratively: Draw from the buffer strip
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);


	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	window::onDestroy();
	m_vb->release();

	m_swap_chain->release();

	GraphicsEngine::get()->release();
}
