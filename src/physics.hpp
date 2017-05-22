/**
	@file	physics.hpp
	@date	2017/5/20
	@author	이성준
	@brief
*/

#pragma once

#include <cmath>
#include "unit.hpp"
#include "map.hpp"

/**
	@class	Physics
	@date	2017/5/21
	@author	이성준
	@brief
*/
class Physics {
private:
	typedef vector<Unit*> UnitList;

	static const float graviational_acceleration_;

	bool IsCollision(Unit *u1, Unit *u2);
	void Reaction(Unit *u1, Unit *u2);

	UnitList *unit_list_;
	Map *map_;
public:
	Physics() {}
	void Init() {};
	void ScanCollision();
	void Gravity(float delta_time);
	void Force(float delta_time);

	void set_unit_list(UnitList *unit_list) { unit_list_ = unit_list; }
};