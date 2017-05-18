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
	void set_model(Model *model) { model_ = model; }
};
