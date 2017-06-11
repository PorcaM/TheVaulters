/**
	@file	rectangle.hpp
	@datea	2017/6/11
	@author	Lee Hyeondong
	@brief
*/

#pragma once

#include <directxmath.h>
#include "transform.hpp"
#include "model.hpp"
#include "rigidbody.hpp"
extern XMMATRIX                g_View;
extern XMMATRIX                g_World;
extern D3D_DRIVER_TYPE         g_driverType;

/**
	@class	Rectangle
	@datea	2017/6/11
	@author	Lee Hyeondong
	@brief
*/
class Rect {
public:
	Rect() {}
	~Rect() {}
	HRESULT Init();
	void Render();

	void setConstantBuffer(ConstantBuffer* cbuffer) { cbuffer_ = cbuffer; }
	void setTranslation(float x, float y) { trns_ = XMFLOAT2(x, y); }
	void setSize(float width, float height) { size_ = XMFLOAT2(width, height); }
	void setPosition(XMFLOAT3 pos) { pos_ = pos; }


	void setRotationX(float angle) { rot_.x = angle; }
	void setRotationY(float angle) { rot_.y = angle; }
	void setRotationZ(float angle) { rot_.z = angle; }
	void rotationApply(bool state) { rot_state_ = state; }

private:
	ID3D11Buffer	*vertex_buffer_		= nullptr;
	ID3D11Buffer	*index_buffer_		= nullptr;
	ConstantBuffer	*cbuffer_			= nullptr;

	XMFLOAT2		trns_				= XMFLOAT2(0.0f, 0.0f);
	XMFLOAT2		size_				= XMFLOAT2(0.0f, 0.0f);
	XMFLOAT3		pos_				= XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMFLOAT3		rot_				= XMFLOAT3(0.0f, 0.0f, 0.0f);

	bool			rot_state_			= false;
};
