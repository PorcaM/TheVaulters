/**
	@file	ai.hpp
	@datea	2017/6/06
	@author	이성준
	@brief
*/

#include "unit.hpp"

class AI
{
public:
	enum Action
	{
		kWalk = 0, kVault, kDodge,
	};

    AI();
    void Init(Unit* unit, Unit* enemy);
    void Update(float delta_time);
    void DetermineAIAction(float delta_time);
    void Rotate();

    void set_unit(Unit *unit);
    void set_enemy(Unit *enemy);

private:
    Unit*               unit_;
    Unit*               enemy_;
    Action              choice_;
    float               delay_;

};