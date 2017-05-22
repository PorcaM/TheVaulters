#include "unit_control.hpp"

void UnitControl::checkKeyState(WPARAM wparam, bool state) {
	if (wparam <= 0x5A && wparam >= 0x41) {
		char key = wparam - 0x41 + 'A';

		switch (key) {
		case 'W':
			vk_w = state;
			break;
		case 'A':
			vk_a = state;
			break;
		case 'S':
			vk_s = state;
			break;
		case 'D':
			vk_d = state;
			break;
		case 'Q':
			vk_q = state;
			break;
		case 'E':
			vk_e = state;
			break;
		}
	}

	if (wparam == VK_SPACE) {
		vk_space = true;
	}
}

void UnitControl::motion_move() {
	float control_value = 0.0f;

	if (vk_w) {
		control_value = unit_->get_transform().position_.z;
		control_value += moving_speed_;
		unit_->set_transform_position_z(control_value);
	}

	if (vk_a) {
		control_value = unit_->get_transform().position_.x;
		control_value -= moving_speed_;
		unit_->set_transform_position_x(control_value);
	}

	if (vk_s) {
		control_value = unit_->get_transform().position_.z;
		control_value -= moving_speed_;
		unit_->set_transform_position_z(control_value);
	}

	if (vk_d) {
		control_value = unit_->get_transform().position_.x;
		control_value += moving_speed_;
		unit_->set_transform_position_x(control_value);
	}
}

void UnitControl::motion_jump(float t) {
	// Initialize state
	if (init_t == 0.0f) {
		init_h = unit_->get_transform().position_.y;
		curr_h = init_h;
		curr_v = init_v;
	}

	// jump
	curr_t = t;
	if (jump_delay < curr_t - init_t) {
		curr_h = unit_->get_transform().position_.y;
		curr_h += curr_v;
		curr_v += gravity;
		unit_->set_transform_position_y(curr_h);

		init_t = curr_t;
	}

	// Initialize state : if jump end
	if (curr_h < init_h) {
		unit_->set_transform_position_y(init_h);
		init_t = 0.0f;
		curr_v = init_v;
		vk_space = false;
	}
}