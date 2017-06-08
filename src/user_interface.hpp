#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "WaveFrontReader.h"
#include "camera_control.hpp"
#include "utility.hpp"

extern ID3D11DeviceContext*    g_pImmediateContext;
extern ID3D11VertexShader*     g_pVertexShader;
extern ID3D11PixelShader*      g_pPixelShader;
extern ID3D11Buffer*           g_pConstantBuffer;
extern ID3D11Device*           g_pd3dDevice;


class UserInterface {
private:
	ID3D11Buffer *vertex_buffer_ = nullptr;
	ID3D11Buffer *index_buffer_ = nullptr;
	ConstantBuffer *cbuffer_ = nullptr;
public:
	UserInterface(ConstantBuffer* cbuffer) {
		vertex_buffer_ = index_buffer_ = nullptr;
		cbuffer_ = cbuffer;
	};
	~UserInterface();

	HRESULT drawRect(float width, float height, float x, float y);

	ID3D11Buffer *get_vertex_buffer() const { return vertex_buffer_; }
	ID3D11Buffer *get_index_buffer() const { return index_buffer_; }

	void Render(int status);
};