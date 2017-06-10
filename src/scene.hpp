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
	virtual void Update() = 0;
	virtual void HandleInput(UINT message, WPARAM wParam, LPARAM lParam) = 0;
};