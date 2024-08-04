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

class AppWindow : public window
{
public:
	AppWindow();

	void updateQuadPosition();

	~AppWindow();

	// Inherited via window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;
	IndexBuffer* m_ib;

private:
	unsigned long m_old_time = 0;
	float m_delta_time = 0;
	float m_angle = 0;

	float m_old_delta = 0;
	float m_new_delta = 0;
	float delta_delta = 0;

	float m_delta_pos = 0;
	float m_delta_scale = 0;
};

