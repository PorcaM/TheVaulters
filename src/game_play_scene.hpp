/**
	@file	game_play_scene.hpp
	@datea	2017/5/16
	@author	이성준
	@brief
*/

#pragma once

#include <vector>
#include <string>
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
class GamePlayScene : public Scene
{
public:
	typedef vector<Model*>	ModelList;
	typedef vector<Unit*>	UnitList;

	struct Theme 
	{
		XMVECTORF32			background_color;
	};
	struct GameInfo
	{
		int					player_cnt;
	};

	enum State {
		kLoad, kPlay, kEnd, kPause
	};

	GamePlayScene() { state_ = State::kLoad; }
	~GamePlayScene();
	virtual HRESULT Init();
	virtual void Render();
	virtual void Update();
	virtual void HandleInput(UINT message, WPARAM wParam, LPARAM lParam);
	void RenderUnitList();
	HRESULT PushUnit(Model *model);

	Camera*					camera_;
	UnitControl*			unit_control_;
	XMFLOAT4				vLightDirs[2];
	XMFLOAT4				vLightColors[2];

private:
	HRESULT InitTheme();
	HRESULT InitModels();
	HRESULT InitUnits();
	HRESULT InitPlayer();
	HRESULT InitMap();
	HRESULT InitLights();
	HRESULT InitConstantBuffer();
	HRESULT InitPhysics();
	HRESULT InitCameraControl();
	bool IsEnd();

	Theme 					theme_;
	State					state_;
	ConstantBuffer 			constant_buffer_;
	ModelList 				model_list_;
	UnitList 				unit_list_;
	Map*					map_;
	Physics 				physics_;
	Player 					player_;
	float					curr_time_;

};