/**
	@file	physics.cpp
	@date	2017/5/21
	@author	�̼���
	@brief
*/

#include "physics.hpp"

void Physics::Init()
{
	InitPhysicalCoefficient();
}

void Physics::InitPhysicalCoefficient()
{
	this->pc_.graviational_acceleration 	= 9.8f;
	this->pc_.minimun_velocity 				= 0.1f;
	this->pc_.ground_resistance 			= 10.0f;
}

void Physics::Update(float delta_time)
{
	Gravity(delta_time);
	Force(delta_time);
	ScanCollision();
	CheckDead();
}

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
		if (IsTerrain(unit) == false) 
		{
			rigidbody.v_.y -= this->pc_.graviational_acceleration * delta_time * factor;
		}
		else 
		{
			// do nothing
		}
		unit->set_rigidbody(rigidbody);
	}
}

void Physics::Force(float delta_time) {
	for (UnitList::iterator it = unit_list_->begin();
		it != unit_list_->end(); it++) {
		Unit *unit = *it;
		Rigidbody rigidbody = unit->get_ridigbody();
		Transform transform = unit->get_transform();

		float delta_x = rigidbody.v_.x * delta_time;
		float delta_y = rigidbody.v_.y * delta_time;
		float delta_z = rigidbody.v_.z * delta_time;
		float prev_y = transform.position_.y;
		transform.position_.x += delta_x;
		transform.position_.y += delta_y;
		transform.position_.z += delta_z;

		if (IsTerrain(unit) && transform.position_.y <= 0) {
			rigidbody.v_.x -= this->pc_.ground_resistance * delta_x;
			rigidbody.v_.z -= this->pc_.ground_resistance * delta_z;
		}
		if (fabs(rigidbody.v_.x) <= this->pc_.minimun_velocity) rigidbody.v_.x = 0;
		if (fabs(rigidbody.v_.z) <= this->pc_.minimun_velocity) rigidbody.v_.z = 0;

		if (IsTerrain(unit) && transform.position_.y < 0.0f) {
			transform.position_.y = 0;
			rigidbody.v_.y = 0;
		}

		unit->set_transform(transform);
		unit->set_rigidbody(rigidbody);

		// Update state if idle
		if (fabs(rigidbody.v_.x) <= this->pc_.minimun_velocity &&
			fabs(rigidbody.v_.y) <= this->pc_.minimun_velocity &&
			fabs(rigidbody.v_.z) <= this->pc_.minimun_velocity)
		{
			unit->set_state(Unit::State::kIdle);
		}
	}
}

bool Physics::IsCollision(Unit *u1, Unit *u2) {
	XMFLOAT3 base = u1->get_transform().position_;
	XMFLOAT3 offset = u2->get_transform().position_;

	// Compare y axis
	if (fabs(base.y - offset.y) > u1->get_ridigbody().height_) 
	{
		return false;
	}

	// Compare projection world (x and y axis)
	base.y = offset.y = 0;
	XMVECTOR vector1 = XMLoadFloat3(&base);
	XMVECTOR vector2 = XMLoadFloat3(&offset);
	XMVECTOR diff = XMVectorSubtract(vector1, vector2);
	XMVECTOR length = XMVector3Length(diff);

	float distance = 0.0f;
	XMStoreFloat(&distance, length);

	if (distance > (u1->get_ridigbody().radius_ + u2->get_ridigbody().radius_))
	{
		return false;
	}

	return true;
}

void Physics::Reaction(Unit *unit1, Unit *unit2) {
	if (unit1->get_state() != Unit::State::kReaction &&
		unit2->get_state() != Unit::State::kReaction)
	{
		Rigidbody rigidbody1 = unit1->get_ridigbody();
		Rigidbody rigidbody2 = unit2->get_ridigbody();
		XMFLOAT3 v1 = rigidbody1.v_;
		XMFLOAT3 v2 = rigidbody2.v_;
		rigidbody1.v_ = v2;
		rigidbody2.v_ = v1;
		unit1->set_rigidbody(rigidbody1);
		unit2->set_rigidbody(rigidbody2);

		// Update state
		unit1->set_state(Unit::State::kReaction);
		unit2->set_state(Unit::State::kReaction);
	}
}

bool Physics::IsTerrain(Unit *unit) {
	Transform transform = unit->get_transform();
	// return map_->TerrainExist(unit->get_transform().position_);
	XMFLOAT3 position = transform.position_;
	if (position.y > 0.0f) return false;

	// Culling
	int length = map_->Length();
	float r = map_->get_r();
	float x_min = map_->get_terrain(0)->position.x - r;
	float z_min = map_->get_terrain(0)->position.z - (r * sqrtf(3) / 2.0f);
	float x_max = map_->get_terrain(length-1)->position.x + r;
	float z_max = map_->get_terrain(length-1)->position.z + (r * sqrtf(3) / 2.0f);
	if ((position.x <= x_min || position.x >= x_max) ||
		(position.z <= z_min || position.z >= z_max))
	{
		return false;
	}
	
	return true;
}

void Physics::CheckDead()
{
	for (UnitList::iterator it = this->unit_list_->begin();
		it != this->unit_list_->end(); it++) 
	{
		Unit* unit = *it;
		float deadline = this->deadline_;
		if (unit->get_transform().position_.y < deadline)
		{
			unit->set_state(Unit::State::kDead);
		}
	}
}