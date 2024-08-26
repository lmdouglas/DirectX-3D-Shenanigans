#pragma once
#include "window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "InputListener.h"
#include "Matrix4x4.h"

class AppWindow : public window, public InputListener
{
public:
	AppWindow();
	~AppWindow();

	// Inherited via window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	// Inherited via InputListener
	void onKeyDown(int key) override;
	void onKeyUp(int key) override;
	void onMouseMove(const Point& mouse_pos) override;
	void onLeftMouseDown(const Point& mouse_pos) override;
	void onLeftMouseUp(const Point& mouse_pos) override;
	void onRightMouseDown(const Point& mouse_pos) override;
	void onRightMouseUp(const Point& mouse_pos) override;

public:
	void update();
	void updateCamera();
	void updateModel();
	void updateSkyBox();
	void drawMesh(const MeshPtr& mesh, VertexShaderPtr& vs, PixelShaderPtr& ps, ConstantBufferPtr& cb, const TexturePtr& tex);

private:
	SwapChainPtr m_swap_chain;
	VertexBufferPtr m_vb;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	ConstantBufferPtr m_cb;
	ConstantBufferPtr m_skybox_cb;
	IndexBufferPtr m_ib;
	TexturePtr m_wood_texture;
	MeshPtr m_mesh;
	MeshPtr m_sky_mesh;
	TexturePtr m_sky_texture;
	TexturePtr m_jupiter_texture;
	PixelShaderPtr m_sky_ps;

private:
	unsigned long m_old_time = 0;
	float m_delta_time = 0;
	float m_angle = 0;

	float m_old_delta = 0;
	float m_new_delta = 0;
	float delta_delta = 0;

	float m_delta_pos = 0;
	float m_delta_scale = 0;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_light_rot_y = 0.0f;

	float m_scale_cube = 1.0f;

	float m_forward = 0.0f;
	float m_right = 0.0f;

	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;

};

