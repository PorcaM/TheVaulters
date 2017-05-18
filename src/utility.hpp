/**
	@file	utility.hpp
	@datea	2017/5/15
	@author	�̼���
	@brief
	@warning	��������� ���� ������ �־�� �� 
				(�Լ� forwading, extern global ���� ��)
*/

#pragma once

#include <fstream>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>

using namespace DirectX;
using namespace std;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vLightDir[2];
	XMFLOAT4 vLightColor[2];
	XMFLOAT4 vOutputColor;
};

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

extern ModelType* m_model;
extern int m_vertexCount, m_indexCount;

extern void LoadModel(char* filename);