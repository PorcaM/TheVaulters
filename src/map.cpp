/**
	@file	map.cpp
	@date	2017/5/16
	@author	¿Ãº∫¡ÿ
	@brief
*/

#include "map.hpp"
#include <time.h>

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
	size_ = scale_ * 85.0f;
}

void Map::Render(ConstantBuffer *contant_buffer) {
	int cnt = 1;
	float light_intencity = 1.0f;
	for (vector<Terrian>::iterator it = terrian_list_.begin();
		it != terrian_list_.end(); it++) {
		// Set local position
		int x = it->index % width_;
		float y = -1.0f;
		int z = it->index / height_;

		// Set constant buffer
		float trans_x = 0.0f;
		if (z % 2 == 1) { trans_x = size_ / 2; }

		if (it->objCode == 1)
			y = -20.0f;

		XMMATRIX translationMatrix = XMMatrixTranslation(x * size_ + trans_x, y, z * (size_ * 0.3f));
		XMMATRIX scaleMatrix = XMMatrixScaling(scale_, scale_, scale_);

		it->Pos.x = x * size_ + trans_x;
		it->Pos.y = z * ( size_ * 0.3f );
		it->size = scale_;

		contant_buffer->vLightDir[cnt] = XMFLOAT4(x * size_ + trans_x, 10.0f, z * (size_ * 0.3f), 1.0f);
		contant_buffer->vLightColor[cnt] = XMFLOAT4(light_intencity, light_intencity, light_intencity, 1.0f);
		contant_buffer->mWorld = XMMatrixTranspose(translationMatrix)*XMMatrixTranspose(scaleMatrix)*XMMatrixTranspose(g_World);
		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, contant_buffer, 0, 0);

		hexagon_->Render();
		if (cnt < 50) {
			cnt++;
		}
		if (light_intencity > 0.0f) {
			light_intencity -= 0.1f;
		}
	}
}