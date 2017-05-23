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
	Rigidbody rigidbody = unit_->get_ridigbody();
	rigidbody.v_.y = 100.0f;
	unit_->set_rigidbody(rigidbody);
	vk_space = false;
}

void UnitControl::Vault() {
	const float power = 500.0f;
	Rigidbody rigidbody = unit_->get_ridigbody();
	Transform transform = unit_->get_transform();
	float x = transform.rotation_.x;
	float y = transform.rotation_.y;
	float z = transform.rotation_.z;

}