/**
	@file	utility.hpp
	@datea	2017/5/15
	@author	ï¿½Ì¼ï¿½ï¿½ï¿½
	@brief
	@warning	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö¾ï¿½ï¿?ï¿½ï¿½ 
				(ï¿½Ô¼ï¿½ forwading, extern global ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½)
*/

#pragma once

#include <fstream>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>

using namespace DirectX;
using namespace std;

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vLightDir[2];
	XMFLOAT4 vLightColor[2];
	XMFLOAT4 vOutputColor;
	float Polish;
};

struct Vertices {
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
};