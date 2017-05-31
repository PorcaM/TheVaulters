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

using namespace std;

/**
	@class	Physics
	@date	2017/5/21
	@author	이성준
	@brief
*/
class Physics {
public:
	typedef vector<Unit*> UnitList;

	Physics() {}
	void Init() {};
	void Update(float delta_time);
	void ScanCollision();
	void Gravity(float delta_time);
	void Force(float delta_time);

	void set_map(Map *map) { map_ = map; }
	void set_unit_list(UnitList *unit_list) { unit_list_ = unit_list; }

private:
	static const float	graviational_acceleration_;
	static const float	minimun_velocity_;

	bool IsCollision(Unit *u1, Unit *u2);
	bool IsTerrain(Unit *unit);
	void Reaction(Unit *u1, Unit *u2);

	UnitList*			unit_list_;
	Map*				map_;

};