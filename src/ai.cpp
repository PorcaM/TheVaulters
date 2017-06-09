/**
	@file	ai.cpp
	@datea	2017/6/06
	@author	이성준
	@brief
*/

#include "ai.hpp"

AI::AI() {}

void AI::Init(Unit* unit, Unit* enemy)
{
	this->delay_ = 2.0f;
    set_unit(unit);
    set_enemy(enemy);
}

void AI::Update(float delta_time)
{
	DetermineAIAction(delta_time);
}

void AI::DetermineAIAction(float delta_time)
{
    this->delay_ -= delta_time;

	if (this->delay_ <= 0)
	{
		this->choice_ = static_cast<Action>(rand() % 3);

		//delay until next action
		this->delay_ = (rand() % 3) + 1.0f;
	}
	Rotate();
	switch (this->choice_)
	{
		default:
			unit_->Jump();
			break;
	}
}

void AI::Rotate()
{
	XMFLOAT3 target = enemy_->get_transform().position_;
	XMFLOAT3 base = unit_->get_transform().position_;

	float y = target.y - base.y;

	base.y = target.y = 0;
	XMVECTOR vector1 = XMLoadFloat3(&base);
	XMVECTOR vector2 = XMLoadFloat3(&target);
	XMVECTOR diff = XMVectorSubtract(vector1, vector2);
	XMVECTOR length = XMVector3Length(diff);
	float distance = 0.0f;
	XMStoreFloat(&distance, length);

	float angle_x = atan2f(y, distance);
	float angle_y = atan2f(target.z - base.z, target.x - base.x);

	this->unit_->set_transform_rotation_x((angle_x));
	this->unit_->set_transform_rotation_y((angle_y/10.0f));
}

void AI::set_unit(Unit *unit)
{
    this->unit_ = unit;
}

void AI::set_enemy(Unit *enemy)
{
    this->enemy_ = enemy;
}