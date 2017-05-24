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

	VirtualKeyboard vk_;
	Unit *unit_;
	UnitControl *unit_control_;

public:
	Player() {}
	void Init();
	void Update();
	void HandleInput(WPARAM w_param, LPARAM l_param, char input_device);

	Unit *get_unit() const { return unit_; }
	UnitControl *get_unit_control() const { return unit_control_; }
	void set_unit(Unit *unit) { unit_ = unit; }
	void set_unit_control(UnitControl *unit_control) { unit_control_ = unit_control; }
};