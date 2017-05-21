/**
	@file	unit.hpp
	@datea	2017/5/16
	@author	이성준
	@brief
*/

#pragma once

#include "transform.hpp"
#include "model.hpp"

extern XMMATRIX                g_World;

/**
	@class	Unit
	@datea	2017/5/16
	@author	이성준
	@brief
*/
class Unit{
private:
	Transform transform_;
	Model *model_;
public:

	Unit() {};
	void Init() {};
	void Render(ConstantBuffer *constant_buffer);

	Transform get_transform() const { return transform_; }
	void set_transform(Transform transform) { transform_ = transform; }
	void set_transform_rotation(XMFLOAT3 rotation) { transform_.rotation_ = rotation; }
	void set_transform_position_x(float position_x) { transform_.position_.x = position_x; }
	void set_transform_position_y(float position_y) { transform_.position_.y = position_y; }
	void set_transform_position_z(float position_z) { transform_.position_.z = position_z; }
	void set_model(Model *model) { model_ = model; }
};
