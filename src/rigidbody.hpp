/**
	@file	rigidbody.hpp
	@date	2017/5/21
	@author	이성준
	@brief
*/

#pragma once

#include <d3d11_1.h>
#include <directxmath.h>

using namespace DirectX;

/**
	@class	Rigidbody
	@date	2017/5/21
	@author	이성준
	@brief
*/
class Rigidbody {
public:
	struct F {
		float		a;
		float		x;
		float		y;
		float		z;
	};
	float			mass_;
	float			width_;		// x
	float			depth_;		// z
	float			height_;	// y
	float			radius_;
	XMFLOAT3		a_;
	XMFLOAT3		v_ = XMFLOAT3(0.0f, 0.0f, 0.0f);

	Rigidbody() {};
	void Init() {
		mass_ = 100.0f;
		width_ = 10.0f;
		depth_ = 10.0f;
		height_ = 10.0f;
		radius_ = 5.0f;
	}
};