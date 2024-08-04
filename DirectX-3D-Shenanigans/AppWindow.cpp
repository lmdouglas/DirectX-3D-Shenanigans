#include "AppWindow.h"
#include <Windows.h>


struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
	vec3 position1;
	vec3 color;
	vec3 color1;
};

//Ensuring our constant time var (for animating) is 16 bytes
__declspec(align(16))
struct constant
{
	float m_angle;
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

	/*
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


		//Two triangles (For TriangleStrip) [startpos1,startpos2,startpos3,endpos1,endpos2,endpos3,color1,color2,color3]
		{-0.5f,-0.5f,0.0f,  -0.32f,-0.11f,0.0f,    1,0,0,   0,0,0}, //bottom left
		{-0.5f,0.5f,0.0f,   -0.11f,0.78f,0.0f,    0,1,0,  1,1,0},//top left
		{0.5f,-0.5f,0.0f,   0.75f,-0.73f,0.0f,    0,0,1,  1,0,0},//bottom right
		{0.5f,0.5f,0.0f,    0.88f,0.77f,0.0f,    1,1,0,  0,0,1} //top right


		//X-Y-Z Original triangle
		//{-0.5f,-0.5f,0.0f}, //POS1
		//{0.0f, 0.5f, 0.0f}, //POS2
		//{0.5f, -0.5f, 0.0f} //POS3
	};
	*/
	vertex list[] =
	{
		//X - Y - Z
		{-0.5f,-0.5f,0.0f,    -0.32f,-0.11f,0.0f,   0,0,0,  0,1,0 }, // POS1
		{-0.5f,0.5f,0.0f,     -0.11f,0.78f,0.0f,    1,1,0,  0,1,1 }, // POS2
		{ 0.5f,-0.5f,0.0f,     0.75f,-0.73f,0.0f,   0,0,1,  1,0,0 },// POS2
		{ 0.5f,0.5f,0.0f,      0.88f,0.77f,0.0f,    1,1,1,  0,0,1 }
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	//GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	//VERTEX SHADER
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs=GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	//PIXEL SHADER
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	//CONSTANTBUFFER
	constant cc;
	cc.m_angle = 0;
	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

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

	//Create constant object
	unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount();

	m_angle += 1.57f * m_delta_time;
	constant cc;
	cc.m_angle = m_angle;

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//Set the default shader in the graphics pipeline to be able to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	//Set the vertices of the triangle to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	//Draw the triangles from list
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0);

	//Altneratively: Draw from the buffer strip
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);


	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	window::onDestroy();
	m_vb->release();

	m_swap_chain->release();

	m_vs->release();
	m_ps->release();

	GraphicsEngine::get()->release();
}
