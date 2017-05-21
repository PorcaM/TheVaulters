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
				break;
			}
		}
	}
}

bool Physics::IsCollision(Unit *u1, Unit *u2) {
	XMFLOAT3 base = u1->get_transform().position_;
	XMFLOAT3 offset = u2->get_transform().position_;
	float width = u1->get_ridigbody().width_;
	float depth = u1->get_ridigbody().depth_;
	float height = u1->get_ridigbody().height_;
	
	if (abs(base.x - offset.x) > width) return false;
	if (abs(base.z - offset.z) > depth) return false;
	if (abs(base.y - offset.y) > height) return false;
	return true;
}

void Physics::Reaction(Unit *u1, Unit *u2) {
	Transform transform = u1->get_transform();
	transform.position_.y += 0.1f;
	u1->set_transform(transform);
}