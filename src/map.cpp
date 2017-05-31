/**
	@file	map.cpp
	@date	2017/5/16
	@author	¿Ãº∫¡ÿ
	@brief
*/

#include "map.hpp"
#include <time.h>

Map::Map() {}

Map::~Map()
{
	for (TerrainList::iterator it = this->terrain_list_.begin();
		it != this->terrain_list_.end(); it++)
	{
		Terrain* terrain = *it;
		delete terrain;
	}
}

void Map::Init(char *mapFile) {
	ifstream file(mapFile);

	// Read meta data
	int depth, width, height;
	file >> depth;
	file >> width >> height;
	
	// Read terrain list
	for (int i = 0; i < width * height; i++) 
	{
		Terrain* terrain		= new Terrain();
		int id;
		file >> id;
		terrain->id				= id;
		terrain->index			= i;
		this->terrain_list_.push_back(terrain);
	}

	// Set map data
	this->depth_				= depth;
	this->width_				= width;
	this->height_				= height;
	this->scale_				= 2.0f;
	this->size_					= scale_ * 85.0f;
}

void Map::Render(ConstantBuffer *contant_buffer) {
	for (TerrainList::iterator it = this->terrain_list_.begin();
		it != this->terrain_list_.end(); it++) 
	{
		Terrain* terrain = *it;

		// Get local index
		int x = terrain->index % this->width_;
		int z = terrain->index / this->height_;

		// Set matrix
		float interval = this->scale_ * this->side_length_;
		float z_offset = (interval * sqrtf(3) / 2) * !(x % 2);
		XMFLOAT3 position = XMFLOAT3(x * interval * 1.5f,
									0,
									z * interval * sqrtf(3) + z_offset);
		XMMATRIX translationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
		XMMATRIX scaleMatrix = XMMatrixScaling(this->scale_, this->scale_, this->scale_);

		// Render hexagon
		contant_buffer->mWorld = 
			XMMatrixTranspose(translationMatrix) *
			XMMatrixTranspose(scaleMatrix) *
			XMMatrixTranspose(g_World);
		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, contant_buffer, 0, 0);
		hexagon_->Render();
	}
}

bool Map::IsTerrain(float x, float z) {
	return true;
}