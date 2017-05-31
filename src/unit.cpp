/**
	@file	unit.cpp
	@datea	2017/5/16
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "unit.hpp"

void Unit::Render(ConstantBuffer *constant_buffer) {
	// Update constant buffer
	Transform transform = this->get_transform();
	XMFLOAT3 position	= transform.position_;
	XMFLOAT2 rotation	= transform.rotation_;
	XMFLOAT3 scale		= transform.scale_;

	XMMATRIX translationMatrix	= XMMatrixTranslationFromVector(XMLoadFloat3(&position));
	XMMATRIX scalingMatrix		= XMMatrixScalingFromVector(XMLoadFloat3(&scale));
	XMMATRIX yawMatrix = XMMatrixRotationRollPitchYaw(0.0f, XMConvertToDegrees(-rotation.y)/10.0f, 0.0f);

	constant_buffer->mWorld = 
		XMMatrixTranspose(translationMatrix) *
		XMMatrixTranspose(scalingMatrix) * 
		XMMatrixTranspose(yawMatrix) *
		XMMatrixTranspose(g_World);

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, constant_buffer, 0, 0);

	model_->Render();
}