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
	void Init(char *mapFile);
	void Render(ConstantBuffer *contant_buffer);
	bool IsTerrain(float x, float z);

	void set_hexagon(Model *hexagon) { hexagon_ = hexagon; side_length_ = hexagon->XExtent(); }

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