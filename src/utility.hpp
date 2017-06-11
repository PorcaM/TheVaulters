/**
	@file	utility.hpp
	@datea	2017/5/15
	@author	�̼���
	@brief
	@warning	���������?���� ������ �־��?�� 
				(�Լ� forwading, extern global ���� ��)
*/

#pragma once

#include <fstream>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

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
	int Polish;
};

struct Vertices {
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
	XMFLOAT3 Normal;
};