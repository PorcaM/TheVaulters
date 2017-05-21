/**
	@file	scene.hpp
	@datea	2017/5/16
	@author	�̼���
	@brief	�������̽� (���� �����Լ��θ� �̷����)
*/

#pragma once

/**
	@class	Scene
	@datea	2017/5/16
	@author	�̼���
	@brief
*/
class Scene {
public:
	Scene() {};
	virtual HRESULT Init() = 0;
	virtual void Render() = 0;
	virtual void HandleInput(WPARAM w_param, LPARAM l_param, char input_device) = 0;
};