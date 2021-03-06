#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "WaveFrontReader.h"
#include "utility.hpp"
#include "DDSTextureLoader.h"

using namespace DirectX;

//extern struct SimpleVertex;
extern ID3D11DeviceContext*    g_pImmediateContext;
extern ID3D11VertexShader*     g_pVertexShader;
extern ID3D11PixelShader*      g_pPixelShader;
extern ID3D11Buffer*           g_pConstantBuffer;
extern ID3D11Device*           g_pd3dDevice;
extern ID3D11PixelShader*      g_pPixelShaderSolid;
extern ID3D11PixelShader*      g_pPixelShader2;
extern ID3D11ShaderResourceView*           g_pTextureRV;
extern ID3D11SamplerState*                 g_pSamplerLinear;

class Model {
public:
	Model() {
		vertex_buffer_ = index_buffer_ = nullptr; index_count_ = 0;
	};
	~Model();

	HRESULT InitFromObj(const wchar_t *obj_file);
	void Render();

	ID3D11Buffer *get_vertex_buffer() const { return vertex_buffer_; }
	ID3D11Buffer *get_index_buffer() const { return index_buffer_; }
	int get_index_count() const { return index_count_; }
	float XExtent() { return x_extent_; }

private:
	ID3D11Buffer*			vertex_buffer_;
	ID3D11Buffer*			index_buffer_;
	int						index_count_;
	UINT					stride_;
	float					x_extent_;
	ID3D11ShaderResourceView*			texture_;

};
