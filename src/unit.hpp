/**
	@file	unit.hpp
	@datea	2017/5/16
	@author	�̼���
	@brief
*/

#pragma once

#include <directxmath.h>
#include "transform.hpp"
#include "model.hpp"
#include "rigidbody.hpp"

extern XMMATRIX                g_World;
extern D3D_DRIVER_TYPE         g_driverType;

/**
	@class	Unit
	@datea	2017/5/16
	@author	�̼���
	@brief
*/
class Unit{
public:
	enum State 
	{
		kIdle, kDead, kWalk, kJump, kVault, kReaction
	};
	enum Direction {
		kForward, kBehind, kLeft, kRight,
	};

	Unit() { state_ = State::kIdle; }
	void Init() { rigidbody_.Init(); }
	void Render(ConstantBuffer *constant_buffer);
	void Move(Direction direction);
	void Jump();
	void Vault(float charge);

	bool IsGround() { return is_ground_; }
	void SetIsGround(bool is_ground) { this->is_ground_ = is_ground; }

	void set_state(State state) 
	{
		if (this->state_ != Unit::State::kDead)
		this->state_ = state; 
	}
	State get_state() { return state_; }
	void setSpeedControl(float speed) { speed_ = speed; }
	Transform get_transform() const { return transform_; }
	Rigidbody get_ridigbody() const { return rigidbody_; }
	float get_transform_rotation_y() { return this->transform_.rotation_.y; }
	float get_transform_rotation_x() { return this->transform_.rotation_.x; }
	void set_transform(Transform transform) { transform_ = transform; }
	void set_rigidbody(Rigidbody rigidbody) { rigidbody_ = rigidbody; }
	void set_transform_position(XMFLOAT3 position) {this->transform_.position_ = position;}
	void set_transform_rotation(XMFLOAT2 rotation) { transform_.rotation_ = rotation; }
	void set_transform_rotation_y(float y) { this->transform_.rotation_.y = y; }
	void set_transform_rotation_x(float x) { this->transform_.rotation_.x = x; }
	void set_transfrom_scale(float scale) {
		this->transform_.scale_.x = this->transform_.scale_.y =
			this->transform_.scale_.z = scale;
	}
	// void set_transform_rotattion_y(float y) { this->transform_.rotation_.y = y; }
	void set_transform_position_x(float position_x) { transform_.position_.x = position_x; }
	void set_transform_position_y(float position_y) { transform_.position_.y = position_y; }
	void set_transform_position_z(float position_z) { transform_.position_.z = position_z; }
	void set_model(Model *model) { model_ = model; }

	float					jump_power_ = 100.0f;
	float					vault_power_ = 500.0f;

private:
	State 					state_;
	Transform 				transform_;
	Rigidbody 				rigidbody_;
	Model*					model_;
	float 					speed_ = 100.0f;
	float 					angle_ = 0.0f;
	bool					is_ground_;
};
