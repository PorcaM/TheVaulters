/**
	@file	unit.cpp
	@datea	2017/5/16
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "unit.hpp"

void Unit::Render(ConstantBuffer *constant_buffer) {
	// Update constant buffer
	float x = get_transform().position_.x;
	float y = get_transform().position_.y;
	float z = get_transform().position_.z;
	XMMATRIX translationMatrix = XMMatrixTranslation(x, y, z);
	constant_buffer->mWorld = XMMatrixTranspose(translationMatrix)*XMMatrixTranspose(g_World);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, constant_buffer, 0, 0);

	model_->Render();
}