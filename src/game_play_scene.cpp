/**
	@file	game_play_scene.cpp
	@date	2017/5/16
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "game_play_scene.hpp"

HRESULT GamePlayScene::Init(){
	HRESULT hr = S_OK;

	hr = InitTheme();
	if (FAILED(hr)) return hr;

	hr = InitModels();
	if (FAILED(hr)) return hr;

	hr = InitUnits();
	if (FAILED(hr)) return hr;

	hr = InitPlayer();
	if (FAILED(hr)) return hr;

	hr = InitMap();
	if (FAILED(hr)) return hr;

	hr = InitLights();
	if (FAILED(hr)) return hr;

	hr = InitConstantBuffer();
	if (FAILED(hr)) return hr;

	hr = InitPhysics();
	if (FAILED(hr)) return hr;

	hr = InitCameraControl();
	if (FAILED(hr)) return hr;

	return S_OK;
}

void GamePlayScene::RenderUnitList() {
	for (UnitList::iterator it = unit_list_.begin();
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

	static float time_prev = 0.0f;
	static float time_curr = 0.0f;
	float delta_time = time_curr - time_prev;
	time_prev = time_curr;
	time_curr = t;

	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, theme_.background_color_);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	// Unit control
	/*UnitControl *player_ctrl = player_.get_unit_control();
	if (player_ctrl->getJumpState()) { player_ctrl->motion_jump(t); }
	player_ctrl->motion_move();*/
	player_.Update();

	physics_.Gravity(delta_time);
	physics_.Force(delta_time);
	physics_.ScanCollision();
	
	// Camera control
	camera_control_->camera_move();
	camera_control_->updateCamera(&constant_buffer_);
	
	RenderUnitList();
	map_->Render(&constant_buffer_);

	g_pSwapChain->Present(0, 0);
}

void GamePlayScene::HandleInput(UINT message, WPARAM wParam, LPARAM lParam) {
	player_.HandleInput(message, wParam, lParam);

	char input_device = 0;
	switch (message) {
	case WM_MOUSEWHEEL:	input_device = 'h';	break;
	case WM_MOUSEMOVE:	input_device = 'm';	break;
	case WM_KEYDOWN:	input_device = 'd';	break;
	case WM_KEYUP:		input_device = 'u';	break;
	}
	if (input_device == 'd') { camera_control_->checkKeyState(wParam, true); }
	if (input_device == 'u') { camera_control_->checkKeyState(wParam, false); }
	if (input_device == 'h') { camera_control_->mouseWheelAction(wParam); }
	if (input_device == 'm') { camera_control_->mouseMoveAction(lParam); }
}

HRESULT GamePlayScene::InitTheme() {
	theme_.background_color_ = Colors::CornflowerBlue;
	return S_OK;
}

HRESULT GamePlayScene::InitModels(){
	model_list_.push_back(new Model());
	model_list_[0]->InitFromObj(L"model/low_poly_sphere.obj");
	model_list_.push_back(new Model());
	model_list_[1]->InitFromObj(L"model/hexagon.obj");
	return S_OK;
}

HRESULT GamePlayScene::InitUnits(){
	unit_list_.push_back(new Unit());
	unit_list_[0]->Init();
	unit_list_[0]->set_model(model_list_[0]);
	unit_list_[0]->set_transform_position_y(30.0f);
	unit_list_.push_back(new Unit());
	unit_list_[1]->Init();
	unit_list_[1]->set_model(model_list_[0]);
	unit_list_[1]->set_transform_position_x(0.0f);
	unit_list_[1]->set_transform_position_z(100.0f);
	return S_OK;
}

HRESULT GamePlayScene::InitPlayer(){
	player_.Init();
	player_.set_unit(unit_list_[0]);
	player_.set_unit_control(new UnitControl());
	player_.get_unit_control()->set_unit(player_.get_unit());
	return S_OK;
}

HRESULT GamePlayScene::InitMap(){
	map_ = new Map();
	map_->Init("map/basic.txt");
	map_->set_hexagon(model_list_[1]);
	return S_OK;
}

HRESULT GamePlayScene::InitLights(){
	vLightDirs[0] = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	vLightDirs[1] = XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f);
	vLightColors[0] = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	vLightColors[1] = XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f);
	return S_OK;
}

HRESULT GamePlayScene::InitConstantBuffer(){
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

HRESULT GamePlayScene::InitPhysics(){
	physics_.set_unit_list(&unit_list_);
	physics_.set_map(map_);
	return S_OK;
}

HRESULT GamePlayScene::InitCameraControl(){
	camera_control_ = new CameraControl(player_.get_unit());
	return S_OK;
}