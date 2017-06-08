/**
	@file	utility.hpp
	@datea	2017/5/15
	@author	이성준
	@brief
	@warning	헤더파일은 선언만 가지고 있어야 함 
				(함수 forwading, extern global 변수 등)
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
};

struct Vertices {
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
};