/**
	@file	unit_control.cpp
	@date	2017/5/21
	@author	ÀÌÇöµ¿
	@brief
*/

#include "unit_control.hpp"

void UnitControl::Move(Direction direction) {
	Transform transform = unit_->get_transform();
	Rigidbody rigidbody = unit_->get_ridigbody();
	float speed = 100.0f;

	if (direction == kForward)	rigidbody.v_.z = speed;
	if (direction == kBehind)	rigidbody.v_.z = -speed;
	if (direction == kLeft)		rigidbody.v_.x = -speed;
	if (direction == kRight)	rigidbody.v_.x = speed;

	/*if (direction == kForward)	transform.position_.z += speed;
	if (direction == kBehind)	transform.position_.z += -speed;
	if (direction == kLeft)		transform.position_.x += -speed;
	if (direction == kRight)	transform.position_.x += speed;*/

	unit_->set_rigidbody(rigidbody);
	unit_->set_transform(transform);
}

void UnitControl::Jump() {
	Rigidbody rigidbody = unit_->get_ridigbody();
	rigidbody.v_.y = 100.0f;
	unit_->set_rigidbody(rigidbody);
}

void UnitControl::Vault() {
	float power = 500.0f;
	Rigidbody rigidbody = unit_->get_ridigbody();
	Transform transform = unit_->get_transform();
	float x = transform.rotation_.x;
	float y = transform.rotation_.y;
	float z = transform.rotation_.z;

}