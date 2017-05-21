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
#include "Unit.hpp"
#include "Map.hpp"

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

	struct Camera {
		XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
		XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	};
	struct Theme {
		XMVECTORF32 background_color_;
	};

	Camera camera_;
	Theme theme_;
	ConstantBuffer constant_buffer_;
	ModelList model_list_;
	UnitList unit_list_;
	Map *map_;
public:
	Unit *player_unit_;
	XMFLOAT4 vLightDirs[2];
	XMFLOAT4 vLightColors[2];

	GamePlayScene(){};
	virtual HRESULT Init();
	virtual void Render();
	virtual void HandleInput(WPARAM w_param);
	void UpdateCamera();
	void RenderUnitList();
};