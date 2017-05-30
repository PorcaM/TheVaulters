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

extern XMMATRIX                g_World;

using namespace std;

/**
	@class	Terrian
	@datea	2017/5/16
	@author	이성준
	@brief
*/
class Terrian{
public:
	int id;
	int index;
	int objCode = 0;
	XMFLOAT2 Pos;
	float size = 0.0f;
};

/**
	@class	Map
	@datea	2017/5/16
	@author	이성준
	@brief
*/
class Map{
private:
	int id_;
	int width_;
	int height_;
	int depth_;
	float scale_;
	float size_;
	vector<Terrian> terrian_list_;
	Model *hexagon_;
public:

	void Init(char *mapFile);
	void Render(ConstantBuffer *contant_buffer);

	vector<Terrian>* getTerrainList() { return &terrian_list_; }

	void set_hexagon(Model *hexagon) { hexagon_ = hexagon; }
};