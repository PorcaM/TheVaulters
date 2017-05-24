/**
	@file	player.cpp
	@date	2017/5/25
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "player.hpp"

void Player::Init() {
	vk_.w = vk_.a = vk_.s = vk_.d = vk_.space = false;
}

void Player::Update() {
	if (vk_.w) unit_control_->Move(UnitControl::Direction::kForward);
	if (vk_.a) unit_control_->Move(UnitControl::Direction::kLeft);
	if (vk_.s) unit_control_->Move(UnitControl::Direction::kBehind);
	if (vk_.d) unit_control_->Move(UnitControl::Direction::kRight);
	if (vk_.space) unit_control_->Jump();
}

void Player::HandleInput(WPARAM w_param, LPARAM l_param, char input_device) {
	bool state;
	if (input_device == 'd')		state = true;
	else if (input_device == 'u')	state = false;

	if (w_param <= 0x5A && w_param >= 0x41) {
		char key = w_param - 0x41 + 'A';
		switch (key) {
		case 'W': vk_.w = state;	break;
		case 'A': vk_.a = state;	break;
		case 'S': vk_.s = state;	break;
		case 'D': vk_.d = state;	break;
		}
	}
	if (w_param == VK_SPACE) {
		vk_.space = state;
	}
}

