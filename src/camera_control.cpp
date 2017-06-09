/**
	@file	camera_control.cpp
	@date	2017/5/21
	@author	ÀÌÇöµ¿
	@brief
*/
#include "camera_control.hpp"

Camera::Camera(Unit* unit) {
	this->unit_ = unit;
}

void Camera::Update(ConstantBuffer* cb) {
	// Set new camera position
	float x = this->unit_->get_transform().position_.x;
	float y = this->unit_->get_transform().position_.y;
	float z = this->unit_->get_transform().position_.z;

	XMVECTOR newEye = XMVectorSet(x, y + 40.0f, z, 0.0f);
	this->view_.Eye = newEye;

	XMVECTOR newAt = XMVectorSet(x, y + 40.0f, z + 50.0f, 0.0f);
	this->view_.At = newAt;

	float ry = this->unit_->get_transform_rotation_y();
	float rx = this->unit_->get_transform_rotation_x();
	XMMATRIX translationMatrix = XMMatrixTranslation(0, 0, -scale);
	XMMATRIX y_rotation_m = XMMatrixRotationRollPitchYaw(0.0f, XMConvertToRadians(ry), 0.0f);
	XMMATRIX rotation_m = XMMatrixRotationX(XMConvertToRadians(-rx));
	XMMATRIX defaultCamera = XMMatrixLookAtLH(this->view_.Eye, this->view_.At, this->view_.Up);

	// Update constant buffer
	XMMATRIX g_View =
		defaultCamera *
		XMMatrixTranspose(y_rotation_m) *
		XMMatrixTranspose(rotation_m)*
		(translationMatrix);
	cb->mView = XMMatrixTranspose(g_View);
}