/**
	@file	player.hpp
	@date	2017/5/22
	@author	이성준
	@brief
*/

#pragma once

#include "unit.hpp"
#include "camera_control.hpp"

/**
	@class	Player
	@date	2017/5/22
	@author	이성준
	@brief
*/
class Player {
public:
	struct VirtualKeyboard {
		bool					w;
		bool					a;
		bool					s;
		bool					d;
		bool					space;
	};
	struct VirtualMouse {
		bool					lb;
		float					x;
		float					y;
		bool					init;
	};

	Player() {}
	void Init();
	void Update();
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam);

	Unit *get_unit() const { return this->unit_; }
	void set_unit(Unit *unit) { this->unit_ = unit; }
	void set_camera(Camera *camera) { this->camera_ = camera; }
	void set_charge(float charge) {
		if (charge > this->max_charge_) charge = this->max_charge_;
		this->charge_ = charge;
	}

private:
	VirtualKeyboard				vk_;
	VirtualMouse				vm_;
	Unit*						unit_;
	Camera*						camera_;
	float						charge_;
	float						charge_speed_;
	float						max_charge_;
	float						rotation_sensitivity_ = 15.0f;

};