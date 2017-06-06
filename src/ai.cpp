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
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	XMVECTOR direction;
	switch (this->choice_)
	{
		default:
			unit_->Jump();
			break;
	}
}

void AI::set_unit(Unit *unit)
{
    this->unit_ = unit;
}

void AI::set_enemy(Unit *enemy)
{
    this->enemy_ = enemy;
}