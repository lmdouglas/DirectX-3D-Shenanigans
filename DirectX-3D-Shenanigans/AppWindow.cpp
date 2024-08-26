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
//This structure can be accessed by shaders
__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
};


AppWindow::AppWindow()
{

}

void AppWindow::update()
{
	//Make sure to update camera first - transforms of models depend on camera matrices
	updateCamera();

	updateModel();

	updateSkyBox();

}

void AppWindow::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;


	//Camera movement: Reduce speed with final term
	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.05f);
	new_pos = new_pos + world_cam.getXDirection() * (m_right * 0.05f);

	//cc.m_camera_position = new_pos;
	world_cam.setTranslation(new_pos); //so as to not be inside our cube

	m_world_cam = world_cam;
	//convert camera matrix into view matrix by inverting the camera matrix
	world_cam.inverse();



	//cc.m_view = world_cam;
	m_view_cam = world_cam;


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

	m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

}

void AppWindow::updateModel()
{
	//Create constant object
	constant cc;

	//Light rotation
	Matrix4x4 m_light_rot_matrix; //4x4 Matrix for light direction
	m_light_rot_matrix.setIdentity();
	m_light_rot_matrix.setRotationY(m_light_rot_y);

	//Rotate light 45deg for next rot
	m_light_rot_y += 0.785f * m_delta_time; //0.785 rads

	cc.m_world.setIdentity();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();
	cc.m_light_direction = m_light_rot_matrix.getZDirection();

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::updateSkyBox()
{
	//Create constant object
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(100.0f, 100.0f, 100.0f)); //Change size of skybox here 

	//Want the sky to move with the camera...
	//Set skybox sphere to position of camera
	cc.m_world.setTranslation(m_world_cam.getTranslation());

	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();

	m_skybox_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::drawMesh(const MeshPtr& mesh, VertexShaderPtr& vs, PixelShaderPtr& ps, ConstantBufferPtr& cb, const TexturePtr& tex)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vs, cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(ps, cb);

	//Set the default shader in the graphics pipeline to be able to draw
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(ps, tex);

	//Set the vertices of the triangle to draw
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());

	//Set the indices of the triangles to draw
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	//Draw the triangles from list
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0);

	//Altneratively: Draw from the buffer strip
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);

	//Draw indexed triangle list
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
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
	m_sky_texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.jpg");

	if (m_wood_texture == nullptr)
	{
		//Error logging?
	}

	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\suzanne.obj");
	//m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Planets\\Jupiter\\jupiter.obj");
	//m_jupiter_texture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Planets\\Jupiter\\2k_jupiter.jpg");

	//Sky mesh
	m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");

		RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	//Inital position of camera
	m_world_cam.setTranslation(Vector3D(0, 0, -1));

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

	//SkyBox Shader
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"SkyBoxShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_sky_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	//CONSTANTBUFFER
	constant cc;
	//cc.m_angle = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));


	//Skybox constant buffer
	m_skybox_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

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
	//Compute transform matrices
	update();
	
	//Set rasterizer: want to render only the front faces of the model (the outside facing ones)
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(false);
	//Render model
	drawMesh(m_mesh, m_vs, m_ps, m_cb, m_sky_texture);

	//Set rasterizer to backface: want to render inside model for skybox
	//Render skybox 
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(true);
	drawMesh(m_sky_mesh, m_vs, m_sky_ps, m_skybox_cb, m_sky_texture);


	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;


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
		m_is_run = false;
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
