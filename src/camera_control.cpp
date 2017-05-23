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

	XMVECTOR newEye = XMVectorSet(x, y, z, 0.0f);
	//camera_.Eye = newEye;

	XMMATRIX translation_m = XMMatrixTranslation(trn_x_, trn_y_, trn_z_);
	XMMATRIX rotation_m = XMMatrixRotationRollPitchYaw(rot_x_, rot_y_,rot_z_);
	XMMATRIX scaling_m = XMMatrixScaling(scale, scale, scale);

	XMMATRIX defaultCamera = XMMatrixLookAtLH(camera_.Eye, camera_.At, camera_.Up);

	XMMATRIX g_View =  translation_m *rotation_m* scaling_m * defaultCamera;

	cb->mView = XMMatrixTranspose(g_View);
}

void CameraControl::checkKeyState(WPARAM wparam, bool state) {
	if (wparam <= 0x5A && wparam >= 0x41) {
		char key = wparam - 0x41 + 'A';

		switch (key) {
		case 'W':
			vk_w = state;
			break;
		case 'A':
			vk_a = state;
			break;
		case 'S':
			vk_s = state;
			break;
		case 'D':
			vk_d = state;
			break;
		}
	}
}

void CameraControl::camera_move() {
	if (vk_w) { trn_z_ -= move_speed_; }
	if (vk_a) { trn_x_ += move_speed_; }
	if (vk_s) { trn_z_ += move_speed_; }
	if (vk_d) { trn_x_ -= move_speed_; }
}

void CameraControl::mouseWheelAction(WPARAM wparam) {
	//float camera_eye_z = XMVectorGetZ(camera_.Eye);

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
		rot_y_ -= (curr_x - prev_x) / sens_rot * PI / 180;
		rot_x_ -= (curr_y - prev_y) / sens_rot * PI / 180;

		prev_x = curr_x;
		prev_y = curr_y;
	}
}