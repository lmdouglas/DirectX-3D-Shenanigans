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

	vertex list[] = {
		//X-Y-Z
		{-0.5f,-0.5f,0.0f}, //POS1
		{0.0f, 0.5f, 0.0f}, //POS2
		{0.5f, -0.5f, 0.0f} //POS3
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	GraphicsEngine::get()->createShaders();
	GraphicsEngine::get()->
	m_vb->load(list, sizeof(vertex), size_list, )
}

void AppWindow::onUpdate()
{
	window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		1, 0, 0, 1);
	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	window::onDestroy();

	m_swap_chain->release();

	GraphicsEngine::get()->release();
}
