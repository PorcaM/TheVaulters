/**
	@file	model.hpp
	@datea	2017/5/15
	@author	이성준
	@brief
*/

#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "utility.hpp"
#include "WaveFrontReader.h"

using namespace DirectX;

//extern struct SimpleVertex;
extern ID3D11DeviceContext*    g_pImmediateContext;
extern ID3D11VertexShader*     g_pVertexShader;
extern ID3D11PixelShader*      g_pPixelShader;
extern ID3D11Buffer*           g_pConstantBuffer;
extern ID3D11Device*           g_pd3dDevice;


/**
	@class	Model
	@date	2017/05/15
	@author	이성준
	@brief	각 모델 객체들
*/
class Model {
private:
	ID3D11Buffer *vertex_buffer_;
	ID3D11Buffer *index_buffer_;
	int index_count_;
	UINT stride_;
public:
	Model() {
		vertex_buffer_ = index_buffer_ = nullptr; index_count_ = 0;
	};
	~Model();

	HRESULT InitFromTxt(char *txt_file);
	HRESULT InitFromObj(const wchar_t *obj_file);
	void Render();

	ID3D11Buffer *get_vertex_buffer() const { return vertex_buffer_; }
	ID3D11Buffer *get_index_buffer() const { return index_buffer_; }
	int get_index_count() const { return index_count_; }
};
