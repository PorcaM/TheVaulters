#include "model.hpp"

Model::~Model() {
	/*if (vertex_buffer_)		vertex_buffer_->Release();
	if (index_buffer_)		index_buffer_->Release();*/
}

HRESULT Model::InitFromObj(const wchar_t *obj_file) {
	typedef UINT Index;
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
	// ������ ������ �ﰢ���� �׷��ִ� 
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);

	// 0611
    // Load the Texture
    hr = CreateDDSTextureFromFile( g_pd3dDevice, L"model/t.dds", nullptr, &texture_);
    if( FAILED( hr ) )
        return hr;

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = g_pd3dDevice->CreateSamplerState( &sampDesc, &g_pSamplerLinear );
    if( FAILED( hr ) )
        return hr;

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
	g_pImmediateContext->PSSetShader(g_pPixelShader2, nullptr, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShaderResources(0, 1, &texture_);
	g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear );
	g_pImmediateContext->DrawIndexed(index_count_*2, 0, 0);
}