/**
	@file	unit.cpp
	@datea	2017/5/16
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "unit.hpp"

void Unit::Render(ConstantBuffer *constant_buffer) {
	// Update our time
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

	// Update constant buffer
	float x = get_transform().position_.x;
	float y = get_transform().position_.y;
	float z = get_transform().position_.z;
	XMMATRIX translationMatrix = XMMatrixTranslation(x, y, z);
	constant_buffer->mWorld = XMMatrixTranspose(translationMatrix)*XMMatrixTranspose(g_World);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, constant_buffer, 0, 0);

	model_->Render();
}