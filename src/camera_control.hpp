/**
@file	camera_control.hpp
@date	2017/5/21
@author	ÀÌÇöµ¿
@brief
*/
#pragma once
#include <directxmath.h>
#include "unit.hpp"
#include "utility.hpp"
class Unit;

class Camera {
public:
	struct View {
		XMVECTOR Eye = XMVectorSet(0.0f, 20.0f, -100.0f, 0.0f);
		XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	};

	Camera(Unit* player_unit);
	void Update(ConstantBuffer* cb);

	Unit* player_unit_;

	// Rotation
	float rot_x_ = 0.0f;
	float yaw = 0.0f;

	// Scaling
	float scale = 0.0f;

	// Mouse action
	float zooming_max = 0.0f;
	float zooming_min = -30.0f;

	float sens_wheel = 2.0f;

	View view_;
};