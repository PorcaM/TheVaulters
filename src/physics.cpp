/**
	@file	physics.cpp
	@date	2017/5/21
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "physics.hpp"

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
			}
		}
	}
}

bool Physics::IsCollision(Unit *u1, Unit *u2) {
	return false;
}

void Physics::Reaction(Unit *u1, Unit *u2) {
	
}