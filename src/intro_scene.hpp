#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "utility.hpp"

extern ID3D11DeviceContext*    g_pImmediateContext;
extern ID3D11VertexShader*     g_pVertexShader;
extern ID3D11PixelShader*      g_pPixelShader;
extern ID3D11Buffer*           g_pConstantBuffer;
extern ID3D11Device*           g_pd3dDevice;
extern D3D_DRIVER_TYPE         g_driverType;

class IntroScene {
private:
	ID3D11Buffer *vertex_buffer_ = nullptr;
	ID3D11Buffer *index_buffer_ = nullptr;
	ConstantBuffer *cbuffer_ = nullptr;
	int scene_number_ = 1;

	/* Prologue */
	float time_delay_ = 3.0f;

	/* Menu */
	float x_ = 0.0f;
	float y_ = 0.0f;
	
	float w_ = 1280.0f;
	float h_ = 720.0f;
	
	bool click_ = false;


public:
	IntroScene(ConstantBuffer* cbuffer) { cbuffer_ = cbuffer; }
	~IntroScene() {}

	HRESULT is_prologue();
	HRESULT is_main_menu();
	HRESULT is_sub_menu();
	HRESULT is_user_information();
	HRESULT is_options();

	HRESULT gps_select_map();
	HRESULT gps_select_character();

	HRESULT drawRect(float width, float height, float x, float y);
	
	void setSceneNumber(int scene_number) { scene_number_ = scene_number; }
	int getSceneNumber() { return scene_number_; }

	void Render();
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam);
};