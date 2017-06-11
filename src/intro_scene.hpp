#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <vector>
#include "utility.hpp"
#include "rect.hpp"
#include "DDSTextureLoader.h"

extern ID3D11DeviceContext*    g_pImmediateContext;
extern ID3D11VertexShader*     g_pVertexShader;
extern ID3D11PixelShader*      g_pPixelShader;
extern ID3D11Buffer*           g_pConstantBuffer;
extern ID3D11Device*           g_pd3dDevice;
extern D3D_DRIVER_TYPE         g_driverType;
extern HWND                    g_hWnd;

class IntroScene {
public:
	typedef vector<Rect*>		RectList;

	IntroScene(ConstantBuffer* cbuffer) { cbuffer_ = cbuffer; }
	~IntroScene();

	HRESULT Init();

	HRESULT is_prologue();
	HRESULT is_main_menu();
	HRESULT is_sub_menu();
	HRESULT is_user_information();
	HRESULT is_options();

	HRESULT gps_select_map();
	HRESULT gps_select_character();
	
	void setSceneNumber(int scene_number) { scene_number_ = scene_number; }
	int getSceneNumber() { return scene_number_; }

	void Render();
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam);

	// if dir is true : max && false : min
	float convertCoord(float position, float size, bool max) {
		if (max)
			return (position * 18.0f) + (size * 18.0f);
		else
			return (position * 18.0f) - (size * 18.0f);
	}
private:
	ID3D11Buffer	*vertex_buffer_ = nullptr;
	ID3D11Buffer	*index_buffer_ = nullptr;
	ConstantBuffer	*cbuffer_ = nullptr;
	RectList		main_menu_;
	POINT			pt_;

	int scene_number_ = 1;

	/* Prologue */
	float time_delay_ = 3.0f;

	/* Menu */
	float x_ = 0.0f;
	float y_ = 0.0f;

	float w_ = 1280.0f;
	float h_ = 720.0f;

	bool click_ = false;
};