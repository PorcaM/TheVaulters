#include "user_interface.hpp"

HRESULT UserInterface::drawRect(float width, float height, float x, float y) {
	HRESULT hr = S_OK;

	Vertices vertices[] = {
		{ XMFLOAT3(1.0f,  1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
	};

	//vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertices) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &vertex_buffer_);
	if (FAILED(hr))
		return hr;

	// Set vertex buffer
	UINT stride = sizeof(Vertices);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);

	// Create index buffer
	UINT indices[] = {
		0,1,2,
		0,3,2,

		0,1,2,
		0,3,2,
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * 12;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	InitData.pSysMem = indices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &index_buffer_);
	if (FAILED(hr))
		return hr;

	// Set index buffer
	g_pImmediateContext->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ);

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);

	UINT stride_ = sizeof(Vertices);

	// XMMATRIX translationMatrix = XMMatrixTranslation(-1.0f, 40.0f, 5.0f);
	XMMATRIX translationMatrix = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX scaleMatrix = XMMatrixScaling(width, height, 1.0f);

	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 10.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX position = XMMatrixLookAtLH(Eye, At, Up);

	//contant_buffer->vLightDir[cnt] = XMFLOAT4(x * size_ + trans_x, 10.0f, z * (size_ * 0.3f), 1.0f);
	//contant_buffer->vLightColor[cnt] = XMFLOAT4(light_intencity, light_intencity, light_intencity, 1.0f);
	cbuffer_->mWorld =
		XMMatrixTranspose(translationMatrix) *
		XMMatrixTranspose(scaleMatrix) *
		XMMatrixTranspose(position);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, cbuffer_, 0, 0);

	offset = 0;

	g_pImmediateContext->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride_, &offset);
	g_pImmediateContext->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R16_UINT, 0);
	g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->DrawIndexed(12, 0, 0);

	return S_OK;
}

void UserInterface::Render(int status) {
	// draw aim
	drawRect(0.1f, 0.8f, 0.0f, 0.0f);
	drawRect(0.8f, 0.1f, 0.0f, 0.0f);
	
	// skill btn
	drawRect(3.0f, 3.0f, -32.0f, -15.0f);

	// status
	for (int cnt = 0; cnt < status; cnt++) {
		drawRect(0.02f, 0.4f, -28.0f + (0.08f * cnt), -17.0f);
	}
}