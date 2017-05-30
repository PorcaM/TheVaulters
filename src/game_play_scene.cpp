/**
	@file	game_play_scene.cpp
	@date	2017/5/16
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "game_play_scene.hpp"

GamePlayScene::~GamePlayScene() 
{
	for (UnitList::iterator it = unit_list_.begin();
		it != unit_list_.end(); it++) 
	{
		delete (*it);
	}
}

HRESULT GamePlayScene::Init(){
	HRESULT hr = S_OK;

	hr = InitTheme();

	hr = InitModels();

	hr = InitUnits();

	hr = InitPlayer();

	hr = InitMap();

	hr = InitLights();

	hr = InitConstantBuffer();

	hr = InitPhysics();

	hr = InitCameraControl();

	state_ = State::kPlay;
	curr_time_ = 0.0f;

	return S_OK;
}

void GamePlayScene::RenderUnitList() {
	for (UnitList::iterator it = unit_list_.begin();
		it != unit_list_.end(); it++) {
		(*it)->Render(&constant_buffer_);
	}
}

void GamePlayScene::Update() {
	if (IsEnd())
	{
		state_ = State::kEnd;
	}
	if (state_ == State::kPlay)
	{
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
		curr_time_ = t;

		static float time_prev = 0.0f;
		static float time_curr = 0.0f;
		float delta_time = time_curr - time_prev;
		time_prev = time_curr;
		time_curr = t;

		player_.Update();

		physics_.Update(delta_time);

		// Camera control
		camera_control_->updateCamera(&constant_buffer_);
	}
}

void GamePlayScene::Render() {
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, theme_.background_color);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
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
	if (input_device == 'h') { camera_control_->mouseWheelAction(wParam); }
	if (input_device == 'm') { camera_control_->mouseMoveAction(lParam); }
}

HRESULT GamePlayScene::InitTheme() {
	theme_.background_color = Colors::CornflowerBlue;
	return S_OK;
}

HRESULT GamePlayScene::InitModels(){
	const int kModelCnt = 2;
	wstring model_path[kModelCnt] = {
		L"model/Muddy.obj",
		L"model/vhexagon2.obj",
	};
	HRESULT hr = S_OK;

	for (int i = 0; i < kModelCnt; i++) {
		model_list_.push_back(new Model());
		hr = model_list_[i]->InitFromObj(model_path[i].c_str());
		if (FAILED(hr)) {
			wstring message = L"Fail to initialize model " + to_wstring(i) + L"\"" + model_path[i] + L"\"";
			MessageBox(nullptr, message.c_str(), L"Error", MB_OK);
			return hr;
		}
	}

	return S_OK;
}

HRESULT GamePlayScene::InitUnits(){
	HRESULT hr = S_OK;
	Model *unit_model = model_list_[0];
	for (int i = 0; i < 2; i++)
	{
		hr = PushUnit(unit_model);
		if (hr == E_FAIL) return hr;
	}

	unit_list_[0]->set_transform_position_y(30.0f);
	Transform transform = unit_list_[0]->get_transform();
	transform.scale_.x = transform.scale_.y = transform.scale_.z = 10.0f;
	unit_list_[0]->set_transform(transform);

	unit_list_[1]->set_transform_position_x(0.0f);
	unit_list_[1]->set_transform_position_z(100.0f);
	transform = unit_list_[1]->get_transform();
	transform.scale_.x = transform.scale_.y = transform.scale_.z = 10.0f;
	transform.rotation_.y = XMConvertToRadians(180.0f);
	unit_list_[1]->set_transform(transform);

	return S_OK;
}

HRESULT GamePlayScene::PushUnit(Model *model) {
	Unit *unit = new Unit();
	if (unit == NULL) return E_FAIL;

	unit->Init();
	unit->set_model(model);

	unit_list_.push_back(unit);

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

bool GamePlayScene::IsEnd() 
{
	int non_dead_cnt = 0;
	for (UnitList::iterator it = unit_list_.begin();
		it != unit_list_.end(); it++)
	{
		Unit *unit = *it;
		if (unit->get_state() != Unit::State::kDead) 
		{
			non_dead_cnt++;
		}
	}
	if (non_dead_cnt > 1)	return false;
	else					return true;
}