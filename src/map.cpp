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

void Map::Init(char *mapFile, Model *model) {
	set_hexagon(model);

	ifstream file(mapFile);

	// Read meta data
	int depth, width, height;
	file >> depth;
	file >> width >> height;
	
	// Set map data
	this->depth_				= depth;
	this->width_				= width;
	this->height_				= height;
	this->scale_				= 2.0f;
	this->size_					= scale_ * 85.0f;

	// Read terrain list
	for (int i = 0; i < width * height; i++) 
	{
		Terrain* terrain		= new Terrain();
		int id;
		file >> id;
		terrain->id				= id;
		terrain->index			= i;
		int x = terrain->index % this->width_;
		int z = terrain->index / this->height_;
		float interval = this->scale_ * this->side_length_;
		float z_offset = (interval * sqrtf(3) / 2) * (x % 2);
		terrain->position.x 	= x * interval * 1.5f;
		terrain->position.y 	= 0;
		terrain->position.z 	= z * interval * sqrtf(3) + z_offset;
		this->terrain_list_.push_back(terrain);
	}

	file.close();
}

void Map::Render(ConstantBuffer *contant_buffer) {
	for (TerrainList::iterator it = this->terrain_list_.begin();
		it != this->terrain_list_.end(); it++) 
	{
		Terrain* terrain = *it;
		XMMATRIX translationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&terrain->position));
		float scale = this->scale_;
		XMMATRIX scaleMatrix = XMMatrixScaling(scale,scale,scale);

		// Render hexagon
		contant_buffer->mWorld = 
			XMMatrixTranspose(translationMatrix) *
			XMMatrixTranspose(scaleMatrix) *
			XMMatrixTranspose(g_World);
		g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, contant_buffer, 0, 0);
		hexagon_->Render();
	}
}

bool Map::TerrainExist(XMFLOAT3 position) {
	for (TerrainList::iterator it = this->terrain_list_.begin();
		it != this->terrain_list_.end(); it++)
	{
		Terrain* terrain = *it;
		XMFLOAT3 base = position;
		XMFLOAT3 offset = terrain->position;

		if (offset.y > 0) continue;

		// Compare projection world (x and y axis)
		base.y = offset.y = 0;
		XMVECTOR vector1 = XMLoadFloat3(&base);
		XMVECTOR vector2 = XMLoadFloat3(&offset);
		XMVECTOR diff = XMVectorSubtract(vector1, vector2);
		XMVECTOR length = XMVector3Length(diff);

		float distance = 0.0f;
		XMStoreFloat(&distance, length);

		if (distance > (this->side_length_))
		{
			continue;
		}
		else return true;
	}
	return false;
}