/**
	@file	scene.hpp
	@datea	2017/5/16
	@author	이성준
	@brief	인터페이스 (순수 가상함수로만 이루어진)
*/

#pragma once

/**
	@class	Scene
	@datea	2017/5/16
	@author	이성준
	@brief
*/
class Scene {
public:
	Scene() {};
	virtual HRESULT Init() = 0;
	virtual void Render() = 0;
	virtual void HandleInput(WPARAM w_param, LPARAM l_param, char input_device) = 0;
};