/**
	@file	player.cpp
	@date	2017/5/25
	@author	�̼���
	@brief
*/

#include "player.hpp"

void Player::Init() {
	vk_.w = vk_.a = vk_.s = vk_.d = vk_.space = false;
	vm_.lb = false;
	vm_.init = true;
	charge_ = 0.0f;
	charge_speed_ = 0.10f;
	max_charge_ = 10.0f;
}

void Player::Update() 
{
	if (this->unit_ == NULL)
	{
		MessageBox(nullptr,
			L"No assigned unit to the player.", L"Error", MB_OK);
	}
	if (vk_.w) 
	{
		if (vk_.a)
			this->unit_->Move(-45.0f);
		else if (vk_.d)
			this->unit_->Move(45.0f);
		else
			this->unit_->Move(0.0f);
	}
	else if (vk_.s)
	{
		if (vk_.a)
			this->unit_->Move(-135.0f);
		else if (vk_.d)
			this->unit_->Move(135.0f);
		else
			this->unit_->Move(180.0f);
	}
	else if (vk_.a) this->unit_->Move(Unit::Direction::kLeft);
	else if (vk_.d) this->unit_->Move(Unit::Direction::kRight);
	if (vk_.space) 
	{ 
		this->unit_->Jump(); 
		vk_.space = false; 
	}
	if (vm_.lb) set_charge(charge_ + charge_speed_);
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
		this->unit_->Vault(charge_);
		break;
	case WM_MOUSEMOVE:
		if (this->vm_.init) 
		{
			this->vm_.x = LOWORD(lParam);
			this->vm_.y = HIWORD(lParam);
			this->vm_.init = false;
		}
		else 
		{
			float curr_x = LOWORD(lParam);
			float curr_y = HIWORD(lParam);

			// Update unit rotation
			float amount = (this->rotation_sensitivity_) / 10.0f;
			float delta_y = (curr_y - this->vm_.y) * amount * 0.3;
			float delta_x = (curr_x - this->vm_.x) * amount * 0.7;
			XMFLOAT2 rotation = this->unit_->get_transform().rotation_;
			rotation.x -= delta_y;
			if (rotation.x > 90.0f) rotation.x = 90.0f;
			if (rotation.x < -90.0f) rotation.x = -90.0f;
			rotation.y += delta_x;
			if (rotation.y > 360.0f) rotation.y -= 360.0f;
			if (rotation.y < 360.0f) rotation.y += 360.0f;
			this->unit_->set_transform_rotation(rotation);

			// Update previous values
			this->vm_.x = LOWORD(lParam);
			this->vm_.y = HIWORD(lParam);
		}
		break;
	case WM_MOUSEWHEEL:
		// Wheel up
		if ((short)HIWORD(wParam) > 0) {
			if (camera_->zooming_max > camera_->scale) { camera_->scale += camera_->sens_wheel; }
			else { camera_->scale = camera_->zooming_max; }
		}

		// Wheel down
		else {
			if (camera_->zooming_min < camera_->scale) { camera_->scale -= camera_->sens_wheel; }
			else { camera_->scale = camera_->zooming_min; }
		}
	}
}

