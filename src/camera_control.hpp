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

class CameraControl {
private:
	struct Camera {
		XMVECTOR Eye = XMVectorSet(0.0f, 20.0f, -100.0f, 0.0f);
		XMVECTOR At  = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR Up  = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	};

	Unit* player_unit_;

	float move_speed_ = 0.1f;

	// Translation
	float trn_x_ = 0.0f;
	float trn_y_ = 0.0f;
	float trn_z_ = 0.0f;

	// Rotation
	float rot_x_ = 0.0f;
	float rot_y_ = 0.0f;
	float rot_z_ = 0.0f;

	// Scaling
	float scale = 1.0f;

	// Mouse action
	float zooming_max = 2.0f;
	float zooming_min = 0.8f;

	float sens_wheel = 0.2f;
	float sens_rot = 10.0f;

	float prev_x = 0.0f;
	float prev_y = 0.0f;

	float curr_x = 0.0f;
	float curr_y = 0.0f;

	bool position_initialize = true;

	// Keyboard
	bool vk_w = false;
	bool vk_a = false;
	bool vk_s = false;
	bool vk_d = false;
	
	Camera camera_;
	
public:
	CameraControl(Unit* player_unit);

	void updateCamera(ConstantBuffer* cb);

	void checkKeyState(WPARAM wparam, bool state);
	void camera_move();

	void mouseWheelAction(WPARAM wparam);
	void mouseMoveAction(LPARAM lparam);

	void setRotationSpeed(float speed) { sens_rot = speed; }
	float getRotationSpeed() { return sens_rot; }
};