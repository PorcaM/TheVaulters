/**
	@file	physics.cpp
	@date	2017/5/21
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "physics.hpp"

const float Physics::graviational_acceleration_ = 9.8f;

void Physics::ScanCollision() {
	for (UnitList::iterator it = unit_list_->begin();
		it != unit_list_->end(); it++) {
		for (UnitList::iterator it2 = unit_list_->begin();
			it2 != unit_list_->end(); it2++) {
			Unit *unit1 = *it;
			Unit *unit2 = *it2;
			if (unit1 == unit2) continue;
			if (IsCollision(unit1, unit2)) {
				Reaction(unit1, unit2);
				break;
			}
		}
	}
}

void Physics::Gravity(float delta_time) {
	static const float factor = 30.0f;
	for (UnitList::iterator it = unit_list_->begin();
		it != unit_list_->end(); it++) {
		Unit *unit = *it;
		Rigidbody rigidbody = unit->get_ridigbody();
		if (unit->get_transform().position_.y > 0) {
			rigidbody.v_.y -= graviational_acceleration_ * delta_time * factor;
		}
		else {
			// rigidbody.v_.y = 0;
		}
		unit->set_rigidbody(rigidbody);
	}
}

void Physics::Force(float delta_time) {
	static const float air_resistance = 10.0f;
	for (UnitList::iterator it = unit_list_->begin();
		it != unit_list_->end(); it++) {
		Unit *unit = *it;
		Rigidbody rigidbody = unit->get_ridigbody();
		float delta_x = rigidbody.v_.x * delta_time;
		float delta_y = rigidbody.v_.y * delta_time;
		float delta_z = rigidbody.v_.z * delta_time;
		Transform transform = unit->get_transform();
		transform.position_.x += delta_x;
		transform.position_.y += delta_y;
		transform.position_.z += delta_z;
		rigidbody.v_.x -= air_resistance * delta_x;
		rigidbody.v_.z -= air_resistance * delta_z;
		if (transform.position_.y < 0) {
			transform.position_.y = 0;
			rigidbody.v_.y = 0;
		}
		unit->set_transform(transform);
		unit->set_rigidbody(rigidbody);
	}
}

bool Physics::IsCollision(Unit *u1, Unit *u2) {
	XMFLOAT3 base = u1->get_transform().position_;
	XMFLOAT3 offset = u2->get_transform().position_;
	float width = u1->get_ridigbody().width_;
	float depth = u1->get_ridigbody().depth_;
	float height = u1->get_ridigbody().height_;
	
	if (abs(base.x - offset.x) > width) return false;
	if (abs(base.z - offset.z) > depth) return false;
	if (abs(base.y - offset.y) > height) return false;
	return true;
}

void Physics::Reaction(Unit *unit1, Unit *unit2) {
	Rigidbody rigidbody1 = unit1->get_ridigbody();
	Rigidbody rigidbody2 = unit2->get_ridigbody();
	XMFLOAT3 v1 = rigidbody1.v_;
	XMFLOAT3 v2 = rigidbody2.v_;
	rigidbody1.v_.x = v2.x;
	rigidbody1.v_.y = v2.y;
	rigidbody1.v_.z = v2.z;
	rigidbody2.v_.x = v1.x;
	rigidbody2.v_.y = v1.y;
	rigidbody2.v_.z = v1.z;
	rigidbody1.v_.y += 10.0f;
	unit1->set_rigidbody(rigidbody1);
	unit2->set_rigidbody(rigidbody2);
}

bool Physics::IsTerrain(Unit *unit) {
	return true;
}