/**
	@file	unit_control.hpp
	@date	2017/5/21
	@author	이현동
	@brief
*/
#pragma once
#include <directxmath.h>
#include "unit.hpp"
#include "rigidbody.hpp"

/**
	@class	UnitControl
	@date	2017/5/21
	@author	이현동
	@brief
*/
class UnitControl {
public:
	enum Direction {
		kForward, kBehind, kLeft, kRight,
	};

	UnitControl() {};
	void Init();
	void Move(Direction direction);
	void Jump();
	void Vault(float charge);

	void set_unit(Unit *unit) { unit_ = unit; }

private:
	Unit* unit_;

};