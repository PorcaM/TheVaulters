/**
	@file	map.cpp
	@date	2017/5/16
	@author	ÀÌ¼ºÁØ
	@brief
*/

#include "map.hpp"

void Map::Init(char *mapFile) {
	ifstream file(mapFile);
	file >> depth_;
	file >> width_ >> height_;
	for (int i = 0; i < width_*height_; i++) {
		Terrian temp;
		file >> temp.id;
		temp.index = i;
		terrian_list_.push_back(temp);
	}
	scale_ = 2.0f;
	size_ = scale_ * 3.0f;
}

void Map::Render(ConstantBuffer *contant_buffer) {
	for (vector<Terrian>::iterator it = terrian_list_.begin();
		it != terrian_list_.end(); it++) {
		// Set local position
		int x = it->index % width_;
		int z = it->index / height_;

		// Set constant buffer
		float trans_x = 0.0f;
		if (z % 2 == 1) { trans_x = size_ / 2; }

		XMMATRIX translationMatrix = XMMatrixTranslation(x * size_ + trans_x, -1, z * (size_ * 0.3f));
		XMMATRIX scaleMatrix = XMMatrixScaling(scale_, scale_, scale_);
		contant_buffer->mWorld = XMMatrixTranspose(translationMatrix)*XMMatrixTranspose(scaleMatrix)*XMMatrixTranspose(g_World);
		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, contant_buffer, 0, 0);

		hexagon_->Render();
	}
}