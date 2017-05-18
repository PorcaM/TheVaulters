/**
	@file	map.체ㅔ
	@datea	2017/5/16
	@author	이성준
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
}

void Map::Render(ConstantBuffer *contant_buffer) {
	for (vector<Terrian>::iterator it = terrian_list_.begin();
		it != terrian_list_.end(); it++) {
		// Set local position
		int x = it->index % width_;
		int z = it->index / height_;
		int size = 10;
		float scale = 5.0f;

		// Set constant buffer
		XMMATRIX translationMatrix = XMMatrixTranslation(x*size, -1, z*size);
		XMMATRIX scaleMatrix = XMMatrixScaling(scale, scale, scale);
		contant_buffer->mWorld = XMMatrixTranspose(translationMatrix)*XMMatrixTranspose(scaleMatrix)*XMMatrixTranspose(g_World);
		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, contant_buffer, 0, 0);

		hexagon_->Render();
	}
}