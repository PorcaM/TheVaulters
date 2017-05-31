/**
	@file	transform.hpp
	@datea	2017/5/16
	@author	이성준
	@brief
*/

#pragma once

#include <d3d11_1.h>
#include <directxmath.h>

using namespace DirectX;

/**
	@class	Transform
	@datea	2017/5/16
	@author	이성준
	@brief	사실상 구조체
*/
class Transform{
public:
	XMFLOAT3 position_;
	XMFLOAT2 rotation_;
	XMFLOAT3 scale_;

	Transform() {
		position_	= XMFLOAT3(0.0f, 0.0f, 0.0f);
		rotation_	= XMFLOAT2(0.0f, 0.0f);
		scale_		= XMFLOAT3(1.0f, 1.0f, 1.0f);
	}
};