/**
	@file	unit.cpp
	@datea	2017/5/16
	@author	�̼���
	@brief
*/

#include "unit.hpp"

void Unit::Render(ConstantBuffer *constant_buffer) {
	// Update constant buffer
	Transform transform = this->get_transform();
	XMFLOAT3 position	= transform.position_;
	XMFLOAT2 rotation	= transform.rotation_;
	XMFLOAT3 scale		= transform.scale_;

	XMMATRIX translationMatrix	= XMMatrixTranslationFromVector(XMLoadFloat3(&position));
	XMMATRIX scalingMatrix		= XMMatrixScalingFromVector(XMLoadFloat3(&scale));
	XMMATRIX yawMatrix 			= XMMatrixRotationRollPitchYaw(0.0f, 
																XMConvertToRadians(rotation.y), 
																0.0f);

	constant_buffer->mWorld = 
		XMMatrixTranspose(translationMatrix) *
		XMMatrixTranspose(scalingMatrix) * 
		XMMatrixTranspose(yawMatrix) *
		XMMatrixTranspose(g_World);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, constant_buffer, 0, 0);

	model_->Render();
}

void Unit::Move(Direction direction) {
	Unit::State state = this->get_state();
	if (state != Unit::State::kIdle && 
		state != Unit::State::kWalk)
		return;

	Transform transform = this->get_transform();
	Rigidbody rigidbody = this->get_ridigbody();
	float speed = this->speed_;
	float yaw = this->get_transform_rotation_y();

	float x = 0.0f;
	float z = 0.0f;
	if (direction == kForward) 
	{
		z += speed * cos(XMConvertToRadians(yaw));
		x += speed * sin(XMConvertToRadians(yaw));
	}
	if (direction == kBehind) 
	{
		z += -speed * cos(XMConvertToRadians(yaw));
		x += -speed * sin(XMConvertToRadians(yaw));
	}
	if (direction == kLeft) 
	{
		x += -speed * cos(XMConvertToRadians(yaw));
		z += speed * sin(XMConvertToRadians(yaw));
	}
	if (direction == kRight) 
	{
		x += speed * cos(XMConvertToRadians(yaw));
		z += -speed * sin(XMConvertToRadians(yaw));
	}
	rigidbody.v_.x = x;
	rigidbody.v_.z = z;
	
	this->set_rigidbody(rigidbody);
	this->set_transform(transform);
	this->set_state(Unit::State::kWalk);
}

void Unit::Jump() {
	Unit::State state = this->get_state();
	if (state != Unit::State::kIdle &&
		state != Unit::State::kWalk) 
		return;

	Rigidbody rigidbody = this->get_ridigbody();
	rigidbody.v_.y = 300.0f;
	this->set_rigidbody(rigidbody);
	this->set_state(Unit::State::kJump);
}

void Unit::Vault(float charge) {
	float power = 500.0f;
	Rigidbody rigidbody = this->get_ridigbody();
	Transform transform = this->get_transform();

	float x = this->get_transform_rotation_x();
	float yaw = this->get_transform_rotation_y();
	rigidbody.v_.z = power * cos(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(x));
	rigidbody.v_.x = power * sin(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(x));
	rigidbody.v_.y = power * sin(XMConvertToRadians(x));

	this->set_rigidbody(rigidbody);
	this->set_state(Unit::State::kVault);
}