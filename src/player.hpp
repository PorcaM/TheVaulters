/**
	@file	player.hpp
	@date	2017/5/22
	@author	이성준
	@brief
*/

#pragma once

#include "unit_control.hpp"

/**
	@class	Player
	@date	2017/5/22
	@author	이성준
	@brief
*/
class Player {
private:
	struct VirtualKeyboard {
		bool w;
		bool a;
		bool s;
		bool d;
		bool space;
	};
	struct VirtualMouse {
		bool lb;
	};

	VirtualKeyboard vk_;
	VirtualMouse vm_;
	Unit *unit_;
	UnitControl *unit_control_;
	float charge_;
	float charge_speed_;
	float max_charge_;

public:
	Player() {}
	void Init();
	void Update();
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam);

	Unit *get_unit() const { return unit_; }
	UnitControl *get_unit_control() const { return unit_control_; }
	void set_unit(Unit *unit) { unit_ = unit; }
	void set_unit_control(UnitControl *unit_control) { unit_control_ = unit_control; }
	void set_charge(float charge) {
		charge_ = charge;
		if (charge_ > max_charge_) charge_ = max_charge_;
	}
};