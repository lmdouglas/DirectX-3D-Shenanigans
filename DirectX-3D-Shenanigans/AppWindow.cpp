#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"

#include "Mesh.h"



struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};

//Ensuring our constant time var (for animating) is 16 bytes
__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_angle = 0.0f;
};


AppWindow::AppWindow()
{

}

void AppWindow::update()
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

	m_delta_scale += m_delta_time / 2.0f; //set the speed here - 2x slower currently

//	cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));
//	temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));
//	cc.m_world *= temp;

	/*
	cc.m_world.setScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));

	temp.setIdentity();
	//temp.setRotationZ(m_delta_scale);
	//For keyboard input rotation:
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	temp.setIdentity();
	//temp.setRotationY(m_delta_scale);
	temp.setRotationY(m_rot_y); //keyboard input rotation
	cc.m_world *= temp;

	temp.setIdentity();
	//temp.setRotationX(m_delta_scale);
	temp.setRotationZ(m_rot_x); //keyboard input rotation
	cc.m_world *= temp;
	*/
	cc.m_world.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;


	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.3f);
	new_pos = new_pos + world_cam.getXDirection() * (m_right * 0.3f);


	world_cam.setTranslation(new_pos); //so as to not be inside our cube

	m_world_cam = world_cam;
	//convert camera matrix into view matrix by inverting the camera matrix
	world_cam.inverse();



	cc.m_view = world_cam;
	/*
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);
	*/

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	//aspect ratio = width of screen / height of screen
    //znear & zfar - distance between origin of camera and near/far planes along the view direction

	cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);




	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

AppWindow::~AppWindow()
{

}

void AppWindow::onCreate()
{
	window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	m_wood_texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");

	if (m_wood_texture == nullptr)
	{
		//Error logging?
	}

	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\teapot.obj");

		RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

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

	/*
	Vector3D position_list[] =
	{
		//Front face
		{Vector3D(-0.5f, -0.5f, -0.5f)},
		{Vector3D(-0.5f, 0.5f,  -0.5f)},
		{Vector3D(0.5f,  0.5f,  -0.5f)},
		{Vector3D(0.5f,  -0.5f, -0.5f)},
		//Back face
		{Vector3D(0.5f,  -0.5f, 0.5f)},
		{Vector3D(0.5f,  0.5f,  0.5f)},
		{Vector3D(-0.5f,  0.5f,  0.5f)},
		{Vector3D(-0.5f, -0.5f, 0.5f)}
	};

	Vector2D texcoord_list[] =
	{
		//Front face
		{Vector2D(0.0f,0.0f)},
		{Vector2D(0.0f,1.0f)},
		{Vector2D(1.0f,0.0f)},
		{Vector2D(1.0f,1.0f)}
	};

	vertex vertex_list[] =
	{
		//CUBE vertices
		//Front face
		{ position_list[0],texcoord_list[1] },
		{ position_list[1],texcoord_list[0] },
		{ position_list[2],texcoord_list[2] },
		{ position_list[3],texcoord_list[3] },
		//Back face
		{ position_list[4],texcoord_list[1] },
		{ position_list[5],texcoord_list[0] },
		{ position_list[6],texcoord_list[2] },
		{ position_list[7],texcoord_list[3] },
		//Face 3
		{ position_list[1],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[2],texcoord_list[3] },
		//Face 4
		{ position_list[7],texcoord_list[1] },
		{ position_list[0],texcoord_list[0] },
		{ position_list[3],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },
		//Face 5
		{ position_list[3],texcoord_list[1] },
		{ position_list[2],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },
		//Face 6
		{ position_list[7],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[1],texcoord_list[2] },
		{ position_list[0],texcoord_list[3] },
	};


	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		//Front side
		0,1,2, //first triangle
		2,3,0, //second triangle
		//Back size
		4,5,6,
		6,7,4,
		//top side
		8,9,10,
		10,11,8,
		//bottom side
		12,13,14,
		14,15,12,
		//right side
		16,17,18,
		18,19,16,
		//left side
		20,21,22,
		22,23,20
	};
	UINT size_index_list = ARRAYSIZE(index_list);

	
	m_ib=GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);
	*/

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	//VERTEX SHADER
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs=GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	//m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	//PIXEL SHADER
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	//CONSTANTBUFFER
	constant cc;
	cc.m_angle = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

}

void AppWindow::onUpdate()
{
	window::onUpdate();

	InputSystem::get()->update();

	//Clear the render target
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()
		->clearRenderTargetColor(
			this->m_swap_chain,
			0,0.3f,0.4f,1);

	//Set viewport of render target in which we have to draw
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	//Update constant buffer
	update();
	
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//Set the default shader in the graphics pipeline to be able to draw
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_wood_texture);

	//Set the vertices of the triangle to draw
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_mesh->getVertexBuffer());

	//Set the indices of the triangles to draw
    GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_mesh->getIndexBuffer());

	//Draw the triangles from list
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0);

	//Altneratively: Draw from the buffer strip
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);

    //Draw indexed triangle list
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();
	delta_delta = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;


	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	window::onDestroy();

}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	//Stop listening to inputs when window is not in focus
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		//m_rot_x += 0.707f * m_delta_time;
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//m_rot_x -= 0.707f * m_delta_time;
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		//m_rot_y += 0.707f * m_delta_time;
		m_right = -1.0f;
	}
	else if (key == 'D')
	{
		//m_rot_y -= 0.707f * m_delta_time;
		m_right = 1.0f;
	}
	else if (key == 27) //ESC key
	{
		//close the window?
	}

}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_right = 0.0f;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_rot_x += (mouse_pos.m_y-(height/2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x-(width/2.0f)) * m_delta_time * 0.1f;


	InputSystem::get()->setCursorPosition(Point(width / 2.0f, height / 2.0f));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;

}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;

}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}
