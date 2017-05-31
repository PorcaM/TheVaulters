/**
	@file	unit_control.cpp
	@date	2017/5/21
	@author	ÀÌÇöµ¿
	@brief
*/

#include "unit_control.hpp"

void UnitControl::Move(Direction direction) {
	Unit::State state = unit_->get_state();
	if (state != Unit::State::kIdle && 
		state != Unit::State::kWalk)
		return;

	Transform transform = unit_->get_transform();
	Rigidbody rigidbody = unit_->get_ridigbody();
	float speed = 100.0f;
	float yaw = unit_->get_transform_rotation_y();

	if (direction == kForward) {
		rigidbody.v_.z = speed * cos(XMConvertToDegrees(-yaw) / 10.0f);
		rigidbody.v_.x = speed * sin(XMConvertToDegrees(-yaw) / 10.0f);
	}

	if (direction == kBehind) {
		rigidbody.v_.z = -speed * cos(XMConvertToDegrees(-yaw) / 10.0f);
		rigidbody.v_.x = -speed * sin(XMConvertToDegrees(-yaw) / 10.0f);
	}

	if (direction == kLeft) {
		rigidbody.v_.x = -speed * cos(XMConvertToDegrees(-yaw) / 10.0f);
		rigidbody.v_.z = speed * sin(XMConvertToDegrees(-yaw) / 10.0f);
	}

	if (direction == kRight) {
		rigidbody.v_.x = speed * cos(XMConvertToDegrees(-yaw) / 10.0f);
		rigidbody.v_.z = -speed * sin(XMConvertToDegrees(-yaw) / 10.0f);
	}
	
	unit_->set_rigidbody(rigidbody);
	unit_->set_transform(transform);
	unit_->set_state(Unit::State::kWalk);
}

void UnitControl::Jump() {
	Unit::State state = unit_->get_state();
	if (state != Unit::State::kIdle &&
		state != Unit::State::kWalk) 
		return;

	Rigidbody rigidbody = unit_->get_ridigbody();
	rigidbody.v_.y = 300.0f;
	unit_->set_rigidbody(rigidbody);
	unit_->set_state(Unit::State::kJump);
}

void UnitControl::Vault(float charge) {
	float power = 3000.0f;
	Rigidbody rigidbody = unit_->get_ridigbody();
	Transform transform = unit_->get_transform();

	float yaw = unit_->get_transform_rotation_y();
	rigidbody.v_.z = power * cos(XMConvertToDegrees(-yaw) / 10.0f);
	rigidbody.v_.x = power * sin(XMConvertToDegrees(-yaw) / 10.0f);

	unit_->set_rigidbody(rigidbody);
	unit_->set_state(Unit::State::kVault);
}