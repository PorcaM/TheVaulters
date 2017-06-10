#include "intro_scene.hpp"

HRESULT IntroScene::is_prologue() {
	static float t = 0.0f;
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 1000.0f;
	}

	if (t > time_delay_) {
		scene_number_ = 1;
		t = 0.0f;
	}

	return S_OK;
}

HRESULT IntroScene::is_main_menu() {
	drawRect(5.0f, 1.0f, 0.0f, -4.0f);
	drawRect(5.0f, 1.0f, 0.0f, -7.0f);
	drawRect(5.0f, 1.0f, 0.0f, -10.0f);
	drawRect(5.0f, 1.0f, 0.0f, -13.0f);

	if (click_ && x_ > -10.0f && x_ < 10.0f) {
		if (y_ > 3.0f && y_ < 10.0f)
			scene_number_ = 10;
		// else if (x)
	}


	return S_OK;
}

HRESULT IntroScene::is_sub_menu() {

	return S_OK;
}

HRESULT IntroScene::is_user_information() {

	return S_OK;
}

HRESULT IntroScene::is_options() {

	return S_OK;
}

HRESULT IntroScene::gps_select_map() {

	return S_OK;
}

HRESULT IntroScene::gps_select_character() {

	return S_OK;
}

void IntroScene::Render() {
	switch (scene_number_) {
	case 0:
		is_prologue();
		break;
	case 1:
		is_main_menu();
		break;
	case 2:
		is_user_information();
		break;
	case 3:
		is_options();
		break;
	case 4:
		exit(true);
		break;
	case 11:
		is_sub_menu();
		break;
	default:
		scene_number_ = 1;
	}
}

void IntroScene::HandleInput(UINT message, WPARAM wParam, LPARAM lParam) {
	GetCursorPos(&pt_);
	ScreenToClient(g_hWnd, &pt_);
	
	switch (message) {
	case WM_LBUTTONDOWN:
		
		// maybe 1.0f = 36px
		x_ = pt_.x - (WINDOW_WIDTH / 2);
		y_ = pt_.y - (WINDOW_HEIGHT / 2);

		if (x_ > -90.0f && x_ < 90.0f) {
			click_ = true;

			if (y_ > convertCoord(4.0f, 1.0f, false) && y_ < convertCoord(4.0f, 1.0f, true))
				scene_number_ = 111;

			else if (y_ > convertCoord(7.0f, 1.0f, false) && y_ < convertCoord(7.0f, 1.0f, true))
				MessageBox(nullptr, L"occured 2", L"Debug", MB_OK);

			else if (y_ > convertCoord(10.0f, 1.0f, false) && y_ < convertCoord(10.0f, 1.0f, true))
				MessageBox(nullptr, L"occured 3", L"Debug", MB_OK);

			else if (y_ > convertCoord(13.0f, 1.0f, false) && y_ < convertCoord(13.0f, 1.0f, true))
				MessageBox(nullptr, L"occured 4", L"Debug", MB_OK);
		}

		break;

	default:
		
		break;
	}
}

HRESULT IntroScene::drawRect(float width, float height, float x, float y) {
	HRESULT hr = S_OK;

	Vertices vertices[] = {
		{ XMFLOAT3( 1.0f,  1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
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

	// Load the Texture
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"seafloor.dds", nullptr, &g_pTextureRV);
	if (FAILED(hr))
		return hr;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
	if (FAILED(hr))
		return hr;

	UINT stride_ = sizeof(Vertices);

	// XMMATRIX translationMatrix = XMMatrixTranslation(-1.0f, 40.0f, 5.0f);
	XMMATRIX translationMatrix = XMMatrixTranslation(x, y, 0.0f);
	XMMATRIX scaleMatrix = XMMatrixScaling(width, height, 1.0f);

	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 10.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX position = XMMatrixLookAtLH(Eye, At, Up);

	cbuffer_->vLightDir[0] = XMFLOAT4(-0.577f, -0.577f, -0.577f, 1.0f);
	cbuffer_->vLightDir[1] = XMFLOAT4(0.0f, 0.0f, 10.0f, 1.0f);
	cbuffer_->vLightColor[0] = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cbuffer_->vLightColor[1] = XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f);

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