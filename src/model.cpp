/**
	@file	model.cpp
	@datea	2017/5/15
	@author	이성준
	@brief
*/

#include "model.hpp"

Model::~Model() {
	/*if (vertex_buffer_)		vertex_buffer_->Release();
	if (index_buffer_)		index_buffer_->Release();*/
}

HRESULT Model::InitFromObj(const wchar_t *obj_file) {
	typedef int Index;
	HRESULT hr = S_OK;
	WaveFrontReader<Index> wfr;
	typedef WaveFrontReader<Index>::Vertex Vertex;
	hr = wfr.Load(obj_file);
	if (FAILED(hr)) {
		wstring message = L"Fail to Load file " + wstring(obj_file);
		MessageBox(nullptr, message.c_str(), L"Error", MB_OK);
		return hr;
	}

	this->x_extent_ = wfr.bounds.Extents.x;

	//vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * wfr.vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &wfr.vertices[0];
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &vertex_buffer_);
	if (FAILED(hr))
		return hr;

	// Set vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);

	// Create index buffer
	// Create vertex buffer

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Index) * wfr.indices.size();        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	InitData.pSysMem = &wfr.indices[0];
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &index_buffer_);
	if (FAILED(hr))
		return hr;

	// Set index buffer
	g_pImmediateContext->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	// 인접된 점끼리 삼각형을 그려주는 
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);

	index_count_ = wfr.indices.size();
	stride_ = sizeof(Vertex);
	return S_OK;
}

void Model::Render() {
	UINT offset = 0;

	g_pImmediateContext->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride_, &offset);
	g_pImmediateContext->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R16_UINT, 0);
	g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(index_count_*2, 0, 0);
}