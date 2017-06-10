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
	Type(AIType::kRandom);
}

void AI::Update(float delta_time)
{
	DetermineAIAction(delta_time);
}

AI::Action AI::Judge()
{
	Action action = Action::kWalk;
	switch(this->Type())
	{
		case AIType::kRandom:
			if (Delay() <= 0.0f)
			{
				do
				{
					action = static_cast<Action>(rand() % 4);
				} while (action == Action::kFar);
				Delay((rand() % 3) + 1.0f);
			}
			break;
		case AIType::kNone:
			return Action::kIdle;
		default:

			break;
	}
	return action;
}

void AI::DetermineAIAction(float delta_time)
{
    this->delay_ -= delta_time;
	Rotate();
	switch (Judge())
	{
		case kCloser:
			unit_->Move(Unit::Direction::kForward);
			break;
		case kFar:
			unit_->Move(Unit::Direction::kBehind);
			break;
		case kVault:
			unit_->Vault(100.0f);
			break;
		case kIdle:
			break;
		default:
			unit_->Move(static_cast<Unit::Direction>(rand() % 4));
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
	float angle_y = atan2f(target.x - base.x, target.z - base.z);

	this->unit_->set_transform_rotation_x(XMConvertToDegrees(angle_x) + 10.0f);
	this->unit_->set_transform_rotation_y(XMConvertToDegrees(angle_y));
}

void AI::Type(AI::AIType type)
{
	this->type_ = type;
}

AI::AIType AI::Type()
{
	return this->type_;
}

void AI::Delay(float delay)
{
	this->delay_ = delay;
}

float AI::Delay()
{
	return this->delay_;
}

void AI::set_unit(Unit *unit)
{
	this->unit_ = unit;
}

void AI::set_enemy(Unit *enemy)
{
	this->enemy_ = enemy;
}