#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "WaveFrontReader.h"
#include "camera_control.hpp"
#include "utility.hpp"
#include "rect.hpp"
#include "unit.hpp"

extern ID3D11DeviceContext*    g_pImmediateContext;
extern ID3D11VertexShader*     g_pVertexShader;
extern ID3D11PixelShader*      g_pPixelShader;
extern ID3D11Buffer*           g_pConstantBuffer;
extern ID3D11Device*           g_pd3dDevice;

class UserInterface {
public:
	typedef vector<Rect*>		RectList;

	UserInterface(ConstantBuffer* cbuffer) { cbuffer_ = cbuffer; }
	~UserInterface();

	HRESULT Init();

	ID3D11Buffer *get_vertex_buffer() const { return vertex_buffer_; }
	ID3D11Buffer *get_index_buffer() const { return index_buffer_; }

	void setUnitPos(Unit* unit) { unit_pos_ = unit->get_ridigbody().v_; }
	void Render(int status);

private:
	ID3D11Buffer	*vertex_buffer_ = nullptr;
	ID3D11Buffer	*index_buffer_ = nullptr;
	ConstantBuffer	*cbuffer_ = nullptr;
	RectList		stuffs_;
	XMFLOAT2		*position_list_;
	XMFLOAT2		*size_list_;
	XMFLOAT3		unit_pos_;

	const int		btnCnt_ = 3;
	const int		stamina_ = 100;
};