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
	XMVECTOR newEye = XMVectorSet(x - 20, y + 70, z - 80.0f, 0.0f);
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
	model_list_[0]->InitFromObj(L"model/low_poly_sphere.obj");
	model_list_.push_back(new Model());
	model_list_[1]->InitFromObj(L"model/hexagon.obj");

	// Init units
	unit_list_.push_back(new Unit());
	unit_list_[0]->Init();
	unit_list_[0]->set_model(model_list_[0]);
	unit_list_[0]->set_transform_position_y(150.0f);
	unit_list_.push_back(new Unit());
	unit_list_[1]->Init();
	unit_list_[1]->set_model(model_list_[0]);
	unit_list_[1]->set_transform_position_x(30.0f);
	unit_list_[1]->set_transform_position_z(30.0f);

	// Init player unit
	player_unit_ = unit_list_[0];

	// Init map
	map_ = new Map();
	map_->Init("map/basic.txt");
	map_->set_hexagon(model_list_[1]);

	// Init lights
	vLightDirs[0] = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	vLightDirs[1] = XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f);
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

	// Init physics system
	physics_.set_unit_list(&unit_list_);

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
	
	UpdateCamera();
	if (jump_state) { jump_motion(t); }
	move_motion();
	physics_.ScanCollision();
	
	physics_.Gravity(delta_time);
	physics_.Force(delta_time);

	RenderUnitList();
	map_->Render(&constant_buffer_);

	g_pSwapChain->Present(0, 0);
}

void GamePlayScene::HandleInput(WPARAM w_param, LPARAM l_param, char input_device) {
	// Keyboard key-down Input
	if (input_device == 'd') {
		if (w_param <= 0x5A && w_param >= 0x41) {
			char key = w_param - 0x41 + 'A';
			float speed = 1.0f;
			float control_value = 0.0f;

			switch (key) {
			case 'W':
				vk_w = true;
				break;
			case 'A':
				vk_a = true;
				break;
			case 'S':
				vk_s = true;
				break;
			case 'D':
				vk_d = true;
				break;
			}
		}

		if (w_param == VK_SPACE) {
			jump_state = true;
		}
	}

	// Keyboard key-up Input
	if (input_device == 'u') {
		if (w_param <= 0x5A && w_param >= 0x41) {
			char key = w_param - 0x41 + 'A';
			float speed = 1.0f;
			float control_value = 0.0f;

			switch (key) {
			case 'W':
				vk_w = false;
				break;
			case 'A':
				vk_a = false;
				break;
			case 'S':
				vk_s = false;
				break;
			case 'D':
				vk_d = false;
				break;
			}
		}
	}

	// Mouse Wheel Input
	if (input_device == 'h') {
		float zooming_max = -50.0f;
		float zooming_min = -100.0f;
		float wheel_sensitivity = 0.2f;
		float camera_eye_z = XMVectorGetZ(camera_.Eye);

		// Wheel up
		if ((short)HIWORD(w_param) > 0) {
			if (zooming_max > camera_eye_z) { camera_eye_z += wheel_sensitivity; }
			else { camera_eye_z = zooming_max; }
		}

		// Wheel down
		else {
			if (zooming_min < camera_eye_z) { camera_eye_z -= wheel_sensitivity; }
			else { camera_eye_z = zooming_min; }
		}

		XMVectorSetZ(camera_.Eye, camera_eye_z);
	}

	// Mouse Movement Input
	if (input_device == 'm') {
		float move_sensitivity = 5.0f;
		float camera_at_x = XMVectorGetX(camera_.At);
		float camera_at_y = XMVectorGetY(camera_.At);
		float difference_x = 0.0f;
		float difference_y = 0.0f;

		XMFLOAT3 control_value = player_unit_->get_transform().rotation_;
		float speed = 0.5f;

		control_value.x += speed;
		player_unit_->set_transform_rotation(control_value);
	}
}

void GamePlayScene::jump_motion(float t) {
	// Initialize state
	if (prev_time == 0.0f) {
		init_position = player_unit_->get_transform().position_.y;
		curr_position = init_position;
		velo = acc;
	}

	// jump
	curr_time = t;
	if (jump_delay < curr_time - prev_time) {
		curr_position = player_unit_->get_transform().position_.y;
		curr_position += velo;
		velo += gravity;
		player_unit_->set_transform_position_y(curr_position);

		prev_time = curr_time;
	}

	// Initialize state : if jump end
	if (curr_position < init_position) {
		player_unit_->set_transform_position_y(init_position);
		prev_time = 0.0f;
		velo = acc;
		jump_state = false;
	}
}

void GamePlayScene::move_motion() {
	float control_value = 0.0f;

	if (vk_w) {
		control_value = player_unit_->get_transform().position_.z;
		control_value += speed;
		player_unit_->set_transform_position_z(control_value);
	}

	if (vk_a) {
		control_value = player_unit_->get_transform().position_.x;
		control_value -= speed;
		player_unit_->set_transform_position_x(control_value);
	}

	if (vk_s) {
		control_value = player_unit_->get_transform().position_.z;
		control_value -= speed;
		player_unit_->set_transform_position_z(control_value);
	}

	if (vk_d) {
		control_value = player_unit_->get_transform().position_.x;
		control_value += speed;
		player_unit_->set_transform_position_x(control_value);
	}
}