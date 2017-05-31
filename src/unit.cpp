/**
	@file	unit.cpp
	@datea	2017/5/16
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "unit.hpp"

void Unit::Render(ConstantBuffer *constant_buffer) {
	// Update constant buffer
	Transform transform = get_transform();
	XMFLOAT3 position	= transform.position_;
	XMFLOAT3 rotation	= transform.rotation_;
	XMFLOAT3 scale		= transform.scale_;
	XMMATRIX translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);

	XMMATRIX scalingMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX yawMatrix = XMMatrixRotationRollPitchYaw(0.0f, XMConvertToDegrees(-yaw_)/10.0f, 0.0f);

	constant_buffer->mWorld = 
		XMMatrixTranspose(translationMatrix) *
		XMMatrixTranspose(scalingMatrix) * 
		XMMatrixTranspose(yawMatrix) *
		XMMatrixTranspose(g_World);

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, constant_buffer, 0, 0);

	model_->Render();
}