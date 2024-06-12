#ifndef SQUARE_H
#define SQUARE_H

#include <Windows.h>
#include <D3D11.h>
#include <d3dx11.h>
#include <D3DX10.h>
#include <xnamath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "D3DX10.lib")

#include "Types.h"
#include "IObject.h" 

struct cbPerObj {
	XMMATRIX WVP;
};

class Square : public IObject {
public:
	Square(ID3D11Device* dev, ID3D11DeviceContext* devcon, XMMATRIX view, XMVECTOR position);
	Square(Square& squ);
	Square &operator=(const Square& squ);
	~Square();

	void Update(XMMATRIX view);
	void Move(XMVECTOR position);
	void Rotate(XMVECTOR axis, float angle);
	void Render ();
private:
	ID3D11DeviceContext* devcon;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* cbPerObjBuffer;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* inputLayout;

	VERTEX vertices[4] = {
		VERTEX(-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		VERTEX(-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		VERTEX( 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		VERTEX( 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f)
	};

	UINT indices[6] = {
		0, 1, 3,
		1, 2, 3
	};

	cbPerObj cbPerObject;

	XMMATRIX world = XMMatrixIdentity();
	XMMATRIX view;
	XMMATRIX proj;
};

#endif