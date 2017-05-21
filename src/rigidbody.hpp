/**
	@file	rigidbody.hpp
	@date	2017/5/21
	@author	이성준
	@brief
*/

#pragma once

/**
	@class	Rigidbody
	@date	2017/5/21
	@author	이성준
	@brief
*/
class Rigidbody {
public:
	struct F {
		float a;
		float x;
		float y;
		float z;
	};
	float mass_;
	float width_;	// x
	float depth_;	// z
	float height_;	// y
	F f_;

	Rigidbody() {};
	void Init() {
		mass_ = 100.0f;
		width_ = 10.0f;
		depth_ = 10.0f;
		height_ = 10.0f;
	}
};