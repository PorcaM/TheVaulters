/**
	@file	camera_control.cpp
	@date	2017/5/21
	@author	ÀÌÇöµ¿
	@brief
*/
#include "camera_control.hpp"

CameraControl::CameraControl(Unit* player_unit) {
	player_unit_ = player_unit;
	move_speed_ = 0.1f;
}

void CameraControl::updateCamera(ConstantBuffer* cb) {
	// Set new camera position
	float x = player_unit_->get_transform().position_.x;
	float y = player_unit_->get_transform().position_.y;
	float z = player_unit_->get_transform().position_.z;

	XMVECTOR newEye = XMVectorSet(x, y + 40.0f, z + scale, 0.0f);
	camera_.Eye = newEye;

	XMVECTOR newAt = XMVectorSet(x, y + 40.0f, z + 50.0f, 0.0f);
	camera_.At = newAt;

	XMMATRIX y_rotation_m = XMMatrixRotationRollPitchYaw(0.0f, XMConvertToDegrees(-yaw) / 10.0f, 0.0f);
	XMMATRIX rotation_m = XMMatrixRotationX(XMConvertToDegrees(-rot_x_)/10.0f);
	XMMATRIX scaling_m = XMMatrixScaling(scale, scale, scale);
	XMMATRIX defaultCamera = XMMatrixLookAtLH(camera_.Eye, camera_.At, camera_.Up);

	XMMATRIX g_View =
		defaultCamera * 
		XMMatrixTranspose(y_rotation_m) * 
		XMMatrixTranspose(rotation_m);

	player_unit_->setYawControl(yaw);
	player_unit_->setSpeedControl(move_speed_);

	cb->mView = XMMatrixTranspose(g_View);
}

void CameraControl::mouseWheelAction(WPARAM wparam) {
	// Wheel up
	if ((short)HIWORD(wparam) > 0) {
		if (zooming_max > scale) { scale += sens_wheel; }
		else { scale = zooming_max; }
	}

	// Wheel down
	else {
		if (zooming_min < scale) { scale -= sens_wheel; }
		else { scale = zooming_min; }
	}
}

void CameraControl::mouseMoveAction(LPARAM lparam) {
	if (position_initialize) {
		prev_x = LOWORD(lparam);
		prev_y = HIWORD(lparam);
		position_initialize = false;
	}

	else {
		curr_x = LOWORD(lparam);
		curr_y = HIWORD(lparam);
# define PI 3.14
		rot_x_ -= (curr_y - prev_y) / sens_rot * PI / 180;
		yaw -= (curr_x - prev_x) / sens_rot * PI / 180;

		prev_x = curr_x;
		prev_y = curr_y;
	}
}