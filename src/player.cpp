/**
	@file	player.cpp
	@date	2017/5/25
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "player.hpp"

void Player::Init() {
	vk_.w = vk_.a = vk_.s = vk_.d = vk_.space = false;
	vm_.lb = false;
	charge_ = 0.0f;
	charge_speed_ = 0.10f;
	max_charge_ = 10.0f;
}

void Player::Update() {
	if (unit_control_ != NULL)
	{
		if (vk_.w) unit_control_->Move(UnitControl::Direction::kForward);
		if (vk_.a) unit_control_->Move(UnitControl::Direction::kLeft);
		if (vk_.s) unit_control_->Move(UnitControl::Direction::kBehind);
		if (vk_.d) unit_control_->Move(UnitControl::Direction::kRight);
		if (vk_.space) { unit_control_->Jump(); vk_.space = false; }
		if (vm_.lb) set_charge(charge_ + charge_speed_);
	}
}

void Player::HandleInput(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		bool state;
		if (message == WM_KEYDOWN)		state = true;
		else if (message == WM_KEYUP)	state = false;

		if (wParam <= 0x5A && wParam >= 0x41) {
			char key = wParam - 0x41 + 'A';
			switch (key) {
			case 'W': vk_.w = state;	break;
			case 'A': vk_.a = state;	break;
			case 'S': vk_.s = state;	break;
			case 'D': vk_.d = state;	break;
			}
		}
		else if (wParam == VK_SPACE) {
			vk_.space = state;
			break;
		}
		break;
	}
	case WM_LBUTTONDOWN:
		vm_.lb = true;
		break;
	case WM_LBUTTONUP:
		vm_.lb = false;
		unit_control_->Vault(charge_);
		break;
	}
}

