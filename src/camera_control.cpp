/**
	@file	camera_control.cpp
	@date	2017/5/21
	@author	ÀÌÇöµ¿
	@brief
*/
#include "camera_control.hpp"

Camera::Camera(Unit* player_unit) {
	player_unit_ = player_unit;
}

void Camera::Update(ConstantBuffer* cb) {
	// Set new camera position
	float x = player_unit_->get_transform().position_.x;
	float y = player_unit_->get_transform().position_.y;
	float z = player_unit_->get_transform().position_.z;

	XMVECTOR newEye = XMVectorSet(x, y + 40.0f, z + scale, 0.0f);
	this->view_.Eye = newEye;

	XMVECTOR newAt = XMVectorSet(x, y + 40.0f, z + 50.0f, 0.0f);
	this->view_.At = newAt;

	float ry = player_unit_->get_transform_rotation_y();
	float rx = player_unit_->get_transform_rotation_x();
	XMMATRIX y_rotation_m = XMMatrixRotationRollPitchYaw(0.0f, XMConvertToDegrees(-ry) / 10.0f, 0.0f);
	XMMATRIX rotation_m = XMMatrixRotationX(XMConvertToDegrees(-rx)/10.0f);
	XMMATRIX scaling_m = XMMatrixScaling(scale, scale, scale);
	XMMATRIX defaultCamera = XMMatrixLookAtLH(this->view_.Eye, this->view_.At, this->view_.Up);

	// Update constant buffer
	XMMATRIX g_View =
		defaultCamera * 
		XMMatrixTranspose(y_rotation_m) * 
		XMMatrixTranspose(rotation_m);
	cb->mView = XMMatrixTranspose(g_View);
}