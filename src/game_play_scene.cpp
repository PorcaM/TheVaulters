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
	switch(this->state_){
		case GamePlayScene::State::kEnd:
		{
			wstring message = L"Player " + to_wstring(this->winner_id_) + L" Win!!";
			MessageBox(nullptr,
			message.c_str(), L"Game End", MB_OK);
			exit(0);
			break;
		}
		case GamePlayScene::State::kPlay:
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

			if (IsEnd())
			{
				state_ = State::kEnd;
			}
			break;
		}
	}
}

void GamePlayScene::Render() {
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, theme_.background_color);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	this->camera_->Update(&this->constant_buffer_);

	RenderUnitList();
	map_->Render(&this->constant_buffer_);

	g_pSwapChain->Present(0, 0);
}

void GamePlayScene::HandleInput(UINT message, WPARAM wParam, LPARAM lParam) {
	// Send input data to the player
	this->player_.HandleInput(message, wParam, lParam);

	// Handle cheat key
	if (message == WM_KEYDOWN)
	{
		if (wParam == 0x31)
		{
			this->unit_list_[0]->set_state(Unit::State::kDead);
		}
		if (wParam == 0x32)
		{
			this->unit_list_[1]->set_state(Unit::State::kDead);
		}
	}
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
	return S_OK;
}

HRESULT GamePlayScene::InitMap(){
	map_ = new Map();
	map_->Init("map/basic.txt", model_list_[1]);
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
	this->camera_ = new Camera(player_.get_unit());
	player_.set_camera(this->camera_);
	return S_OK;
}

bool GamePlayScene::IsEnd() 
{
	int non_dead_cnt = 0;
	int i = 0;
	for (UnitList::iterator it = unit_list_.begin();
		it != unit_list_.end(); it++)
	{
		Unit *unit = *it;
		if (unit->get_state() != Unit::State::kDead) 
		{
			non_dead_cnt++;
			winner_id_ = i;
		}
		i++;
	}
	if (non_dead_cnt > 1)	return false;
	else					return true;
}