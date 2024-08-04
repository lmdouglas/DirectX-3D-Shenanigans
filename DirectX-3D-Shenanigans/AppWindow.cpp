#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"


struct vertex
{
	Vector3D position;
	Vector3D position1;
	Vector3D color;
	Vector3D color1;
};

//Ensuring our constant time var (for animating) is 16 bytes
__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_angle;
};


AppWindow::AppWindow()
{

}

void AppWindow::updateQuadPosition()
{
	//Create constant object
	unsigned long new_time = 0;
	if (m_old_time)
		new_time = ::GetTickCount64() - m_old_time;
	m_delta_time = new_time / 1000.0f;
	m_old_time = ::GetTickCount64();

	m_angle += 1.57f * m_delta_time;


	constant cc;
	cc.m_angle = m_angle;

	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;


	Matrix4x4 temp;

	m_delta_scale += m_delta_time / 0.15f;

	cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));

	temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));

	cc.m_world *= temp;


	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);


	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
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
		{Vector3D( - 0.5f,-0.5f,0.0f),    Vector3D(- 0.32f,-0.11f,0.0f),  Vector3D(0,0,0),  Vector3D(0,1,0)}, // POS1
		{Vector3D(-0.5f,0.5f,0.0f),    Vector3D(-0.11f,0.78f,0.0f),  Vector3D(1,1,0),  Vector3D(0,1,1)}, // POS2
		{Vector3D(0.5f,-0.5f,0.0f),    Vector3D(0.75f,-0.73f,0.0f),  Vector3D(0,0,1),  Vector3D(1,0,0)},  
		{Vector3D(0.5f,0.5f,0.0f),    Vector3D(0.88f,0.77f,0.0f),  Vector3D(1,1,1),  Vector3D(0,0,1)} 

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

	//Update constant buffer
	updateQuadPosition();
	
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

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();
	delta_delta = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;


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
