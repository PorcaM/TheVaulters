/**
@file	unit_control.hpp
@date	2017/5/21
@author	ÀÌÇöµ¿
@brief
*/
#pragma once
#include <directxmath.h>
#include "unit.hpp"
#include "rigidbody.hpp"

class Unit;
#define PI 3.14f

class UnitControl {
private:
	Unit* unit_;

	// Keyboard State : Key-Down = true, Key-Up = false
	bool vk_q = false;
	bool vk_w = false;
	bool vk_e = false;
	bool vk_a = false;
	bool vk_s = false;
	bool vk_d = false;

	bool vk_space = false;

	// global : motion_move
	float moving_speed_ = 0.1f;

	// global : motion_jump
	// height position
	float init_h = 0.0f;
	float curr_h = 0.0f;

	// time
	float init_t = 0.0f;
	float curr_t = 0.0f;

	// velocity
	float init_v = 2.0f;
	float curr_v = 0.0f;

	float gravity = -0.2f;

	float jump_delay = 0.016f;

public:
	UnitControl() {};
	void Init() {};
	void Vault();
	void set_unit(Unit *unit) { unit_ = unit; }

	void checkKeyState(WPARAM wparam, bool state);

	void motion_move();
	void motion_jump(float time);

	void setMovingSpeed(float speed) { moving_speed_ = speed; }
	float getMovingSpeed() { return moving_speed_; }

	// Set the jump destination height coordinate
	void setJumpDestH(float h) { init_h = h; }

	bool getJumpState() { return vk_space; }
};