/**
	@file	player.hpp
	@date	2017/5/22
	@author	�̼���
	@brief
*/

#pragma once

#include "unit_control.hpp"

/**
	@class	Player
	@date	2017/5/22
	@author	�̼���
	@brief
*/
class Player {
private:
	Unit *unit_;
	UnitControl *unit_control_;
public:
	Player() {}
	void Init() {};

	Unit *get_unit() const { return unit_; }
	UnitControl *get_unit_control() const { return unit_control_; }
	void set_unit(Unit *unit) { unit_ = unit; }
	void set_unit_control(UnitControl *unit_control) { unit_control_ = unit_control; }
};