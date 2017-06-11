/**
	@file	game_play_scene.cpp
	@date	2017/5/16
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
	InitUnitsLocation();

	hr = InitLights();

	hr = InitConstantBuffer();

	hr = InitPhysics();

	hr = InitCameraControl();

	InitAI();
	// InitNetwork();

	hr = InitUserInterface();
	
	hr = InitPrevScenes();
	

	state_ = State::kPlay;
	curr_time_ = 0.0f;

	return S_OK;
}

void GamePlayScene::RenderUnitList() {
	int i = 0;
	for (UnitList::iterator it = unit_list_.begin();
		it != unit_list_.end(); it++) {
		if (i == 0) constant_buffer_.vLightColor[1] = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		else constant_buffer_.vLightColor[1] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		(*it)->Render(&constant_buffer_);
		i++;
	}
}

void GamePlayScene::Update() {
	if (intro_->getSceneNumber() != 111) return;
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
			ai_.Update(delta_time);
			// network_.Update();
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
	
	if (intro_->getSceneNumber() != 111) {
		intro_->Render();
		// ui_->Render(100);
	} // End of if (IntroScene)
	else
	{
		this->camera_->Update(&this->constant_buffer_);

		RenderUnitList();
		map_->Render(&this->constant_buffer_);
		ui_->Render(100);
	}
	g_pSwapChain->Present(0, 0);
}

void GamePlayScene::HandleInput(UINT message, WPARAM wParam, LPARAM lParam) {
	// Send input data to the player
	if (intro_->getSceneNumber() != 111) {
      this->intro_->HandleInput(message, wParam, lParam);
   } // End of if (IntroScene)
   else {
      this->player_.HandleInput(message, wParam, lParam);
	  // this->network_.Communication(message, wParam, lParam);
   }

	// Handle cheat key
	if (message == WM_KEYDOWN)
	{
		float speed = 0.1f;
		XMFLOAT4 temp = constant_buffer_.vLightDir[0];
		if (wParam == 0x31)
		{			
			temp.x += speed;			
		}
		if (wParam == 0x32)
		{
			temp.x -= speed;
		}
		if (wParam == 0x33)
		{
			temp.y += speed;
		}
		if (wParam == 0x34)
		{
			temp.y -= speed;
		}if (wParam == 0x35)
		{
			temp.z += speed;
		}
		if (wParam == 0x36)
		{
			temp.z -= speed;
		}
		if (wParam == 0x37)
		{
			temp.z += 0;
			// debugging
		}
		constant_buffer_.vLightDir[0] = temp;
		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &constant_buffer_, 0, 0);
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
		unit_list_[i]->set_transfrom_scale(10.0f);
	}
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

	// added (0611)
	enemy_.Init();
	enemy_.set_unit(unit_list_[1]);
	// added line end
	return S_OK;
}

HRESULT GamePlayScene::InitMap(){
	map_ = new Map();
	map_->Init("map/basic.txt", model_list_[1]);
	return S_OK;
}

HRESULT GamePlayScene::InitUnitsLocation()
{
	srand(time(NULL));
	for (UnitList::iterator it = this->unit_list_.begin();
		it != this->unit_list_.end(); it++)
	{
		Unit* unit = *it;
		int length = map_->Length();
		int index = rand() % length;
		Terrain* terrain = map_->get_terrain(index);
		unit->set_transform_position(terrain->position);
	}
	return S_OK;
}

HRESULT GamePlayScene::InitLights(){
	//vLightDirs[0] = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	vLightDirs[0] = XMFLOAT4( -0.5f, 0, -15, 1.0f );
	vLightDirs[1] = XMFLOAT4(1, 1, -1, 1.0f);
	vLightColors[0] = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	vLightColors[1] = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
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
	constant_buffer_.vOutputColor	= XMFLOAT4(0, 1.0f, 0.0f, 0.0f);
	constant_buffer_.Polish			= 1.0f;
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &constant_buffer_, 0, 0);
	return S_OK;
}

HRESULT GamePlayScene::InitPhysics(){
	physics_.Init();
	physics_.set_unit_list(&unit_list_);
	physics_.set_map(map_);
	return S_OK;
}

HRESULT GamePlayScene::InitCameraControl(){
	this->camera_ = new Camera(player_.get_unit());
	player_.set_camera(this->camera_);
	return S_OK;
}

HRESULT GamePlayScene::InitAI()
{
	ai_.Init(unit_list_[1], unit_list_[0]);
	ai_.Type(AI::AIType::kNone);
	return S_OK;
}

HRESULT GamePlayScene::InitNetwork()
{
	network_.Init();
	network_.SetUnits(unit_list_[0], unit_list_[1]);
	// added (0611)
	network_.setEnemys(&enemy_);
	// added line end
	return S_OK;
}

HRESULT GamePlayScene::InitUserInterface() {
	ui_ = new UserInterface(&constant_buffer_);
	ui_->Init();
	ui_->setUnitPos(unit_list_[0]); 

	return S_OK;
}

HRESULT GamePlayScene::InitPrevScenes() {
	intro_ = new IntroScene(&constant_buffer_);
	intro_->Init();

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