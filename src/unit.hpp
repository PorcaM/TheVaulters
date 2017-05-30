/**
	@file	unit.hpp
	@datea	2017/5/16
	@author	이성준
	@brief
*/

#pragma once

#include "transform.hpp"
#include "model.hpp"
#include "rigidbody.hpp"

extern XMMATRIX                g_World;
extern D3D_DRIVER_TYPE         g_driverType;

/**
	@class	Unit
	@datea	2017/5/16
	@author	이성준
	@brief
*/
class Unit{
public:
	enum State 
	{
		kIdle, kDead, kWalk, kVault
	};

	Unit() { state_ = State::kIdle; }
	void Init() { rigidbody_.Init(); }
	void Render(ConstantBuffer *constant_buffer);

	void setYawControl(float yaw) { yaw_ = yaw; }
	float getYawControl() { return yaw_; }
	void setSpeedControl(float speed) { speed_ = speed; }

	Transform get_transform() const { return transform_; }
	Rigidbody get_ridigbody() const { return rigidbody_; }
	void set_transform(Transform transform) { transform_ = transform; }
	void set_rigidbody(Rigidbody rigidbody) { rigidbody_ = rigidbody; }
	void set_transform_rotation(XMFLOAT3 rotation) { transform_.rotation_ = rotation; }
	void set_transform_position_x(float position_x) { transform_.position_.x = position_x; }
	void set_transform_position_y(float position_y) { transform_.position_.y = position_y; }
	void set_transform_position_z(float position_z) { transform_.position_.z = position_z; }
	void set_model(Model *model) { model_ = model; }

private:
	State 					state_;
	Transform 				transform_;
	Rigidbody 				rigidbody_;
	Model*					model_;
	float 					yaw_ = 0.0f;
	float 					speed_ = 0.0f;
	float 					angle_ = 0.0f;
};
