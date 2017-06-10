/**
	@file	ai.hpp
	@datea	2017/6/06
	@author	이성준
	@brief
*/

#pragma once

#include "unit.hpp"

class AI
{
public:
	enum Action
	{
		kWalk = 0, kVault, kCloser, kFar, kIdle,
	};
	enum AIType
	{
		kRandom, kNone
	};

    AI();
    void Init(Unit* unit, Unit* enemy);
    void Update(float delta_time);
    AI::Action Judge();
    void DetermineAIAction(float delta_time);
    void Rotate();

    void Type(AI::AIType type);
    AI::AIType Type();
    void Delay(float delay);
    float Delay();
    void set_unit(Unit *unit);
    void set_enemy(Unit *enemy);

private:
    Unit*               unit_;
    Unit*               enemy_;
    Action              choice_;
    float               delay_;
    AIType              type_;
};

