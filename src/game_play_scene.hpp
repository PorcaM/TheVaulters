/**
	@file	game_play_scene.hpp
	@datea	2017/5/16
	@author	이성준
	@brief
*/

#pragma once

#include <vector>
#include <d3d11_1.h>
#include <directxcolors.h>
#include "scene.hpp"
#include "unit.hpp"
#include "map.hpp"
#include "physics.hpp"
#include "camera_control.hpp"
#include "unit_control.hpp"
#include "player.hpp"

using namespace std;
using namespace DirectX;

extern D3D_DRIVER_TYPE         g_driverType;
extern IDXGISwapChain*         g_pSwapChain;
extern ID3D11RenderTargetView* g_pRenderTargetView;
extern ID3D11DepthStencilView* g_pDepthStencilView;
extern XMMATRIX                g_World;
extern XMMATRIX                g_View;
extern XMMATRIX                g_Projection;

/**
	@class	GamePlayScene
	@datea	2017/5/16
	@author	이성준
	@brief
*/
class GamePlayScene : public Scene{
private:
	typedef vector<Model*> ModelList;
	typedef vector<Unit*> UnitList;

	struct Theme {
		XMVECTORF32 background_color_;
	};

	Theme theme_;
	ConstantBuffer constant_buffer_;
	ModelList model_list_;
	UnitList unit_list_;
	Map *map_;
	Physics physics_;
	Player player_;
public:
	CameraControl *camera_control_;
	UnitControl *unit_control_;
	XMFLOAT4 vLightDirs[2];
	XMFLOAT4 vLightColors[2];

	GamePlayScene(){};
	virtual HRESULT Init();
	virtual void Render();
	virtual void HandleInput(WPARAM w_param, LPARAM l_param, char input_device);
	void RenderUnitList();
};