/**
	@file	map.hpp
	@datea	2017/5/16
	@author	이성준
	@brief
*/

#pragma once

#include <vector>
#include <fstream>
#include "utility.hpp"
#include "model.hpp"

using namespace std;

extern XMMATRIX                g_World;

/**
	@class	Terrian
	@datea	2017/5/16
	@author	이성준
	@brief
*/
class Terrain{
public:
	int						id;
	int						index;
	int						obj_code = 0;
	XMFLOAT3				position;
	float					size = 0.0f;
};

/**
	@class	Map
	@datea	2017/5/16
	@author	이성준
	@brief
*/
class Map{
public:
	typedef vector<Terrain*> TerrainList;

	Map();
	~Map();
	void Init(const char *path, Model *hexagon);
	void ParseMapFile(const char *path);
	void Render(ConstantBuffer *contant_buffer);
	bool TerrainExist(XMFLOAT3 position);
	
	Terrain* get_terrain(int index)
	{
		int i = index / this->width_;
		int j = index % this->width_;
		int new_index = (i+1) * (this->width_ + 2) + (j+1);
		return this->terrain_list_[new_index];
	}
	int get_width() { return this->width_; }
	int get_height() { return this->height_; }
	int get_r() { return this->side_length_; }
	int Length()
	{
		return this->width_ * this->height_;
	}
	void set_hexagon(Model *hexagon)
	{
		this->hexagon_ = hexagon;
		this->side_length_ = hexagon->XExtent();
	}


	float max_x_ = 0;
	float min_x_ = 10000;
	float max_z_ = 0;
	float min_z_ = 10000;

private:
	int						id_;
	int						width_;
	int						height_;
	int						depth_;
	float					scale_;
	float					size_;
	float					side_length_;
	
	vector<Terrain*>		terrain_list_;
	Model*					hexagon_;
};