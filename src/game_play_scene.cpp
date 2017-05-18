/**
	@file	game_play_scene.cpp
	@datea	2017/5/16
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "game_play_scene.hpp"


void GamePlayScene::UpdateCamera() {
	// Set new camera position
	float x = player_unit_->get_transform().position_.x;
	float y = player_unit_->get_transform().position_.y;
	float z = player_unit_->get_transform().position_.z;
	XMVECTOR newEye = XMVectorSet(x, y, z - 30.0f, 0.0f);
	camera_.Eye = newEye;

	// Update camera position to directx
	g_View = XMMatrixLookAtLH(camera_.Eye, camera_.At, camera_.Up);
	constant_buffer_.mView = XMMatrixTranspose(g_View);
}

HRESULT GamePlayScene::Init(){
	// Init theme
	theme_.background_color_ = Colors::CornflowerBlue;

	// Init models
	model_list_.push_back(new Model());
	model_list_[0]->InitFromTxt("model/tube.txt");
	model_list_.push_back(new Model());
	model_list_[1]->InitFromTxt("model/star.txt");

	// Init units
	unit_list_.push_back(new Unit());
	unit_list_[0]->set_model(model_list_[0]);
	unit_list_.push_back(new Unit());
	unit_list_[1]->set_model(model_list_[1]);

	// Init player unit
	player_unit_ = unit_list_[0];
	// player_unit_->get_transform().position_.y = 10.0f;

	// Init map
	map_ = new Map();
	map_->Init("map/basic.txt");
	map_->set_hexagon(model_list_[1]);

	// Init lights
	vLightDirs[0] = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	vLightDirs[1] = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	vLightColors[0] = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	vLightColors[1] = XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f);

	// Init constant buffer
	constant_buffer_.mWorld			= XMMatrixTranspose(g_World);
	constant_buffer_.mView			= XMMatrixTranspose(g_View);
	constant_buffer_.mProjection	= XMMatrixTranspose(g_Projection);
	constant_buffer_.vLightDir[0]	= vLightDirs[0];
	constant_buffer_.vLightDir[1]	= vLightDirs[1];
	constant_buffer_.vLightColor[0] = vLightColors[0];
	constant_buffer_.vLightColor[1] = vLightColors[1];
	constant_buffer_.vOutputColor	= XMFLOAT4(0, 0, 0, 0);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &constant_buffer_, 0, 0);

	return S_OK;
}

void GamePlayScene::RenderUnitList() {
	for (vector<Unit*>::iterator it = unit_list_.begin();
		it != unit_list_.end(); it++) {
		(*it)->Render(&constant_buffer_);
	}
}

void GamePlayScene::Render() {
	// Update our time
	static float t = 0.0f;
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 1000.0f;
	}

	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, theme_.background_color_);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	UpdateCamera();

	RenderUnitList();
	map_->Render(&constant_buffer_);

	g_pSwapChain->Present(0, 0);
}

void GamePlayScene::HandleInput(WPARAM w_param) {
	if (w_param <= 0x5A && w_param >= 0x41) {
		char key = w_param - 0x41 + 'A';
		float speed = 0.5f;
		switch (key) {
		case 'W':
			break;
		case 'A':
			//player_unit_->get_transform()->position_.x -= speed;
			break;
		case 'S':
			//player_unit_->get_transform()->position_.z -= speed;
			break;
		case 'D':
			//player_unit_->get_transform()->position_.x += speed;
			break;
		}
	}
}