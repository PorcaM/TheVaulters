/**
	@file	physics.hpp
	@date	2017/5/20
	@author	이성준
	@brief
*/

#pragma once

#include "unit.hpp"

/**
	@class	Physics
	@date	2017/5/21
	@author	이성준
	@brief
*/
class Physics {
private:
	typedef vector<Unit*> UnitList;

	bool IsCollision(Unit *u1, Unit *u2);
	void Reaction(Unit *u1, Unit *u2);

	UnitList *unit_list_;
public:
	Physics() {}
	void Init() {};
	void ScanCollision();

	void set_unit_list(UnitList *unit_list) { unit_list_ = unit_list; }
};