#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <Windows.h>
#include <D3D11.h>
#include <d3dx11.h>
#include <D3DX10.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "D3DX10.lib")

#include "Types.h"

class Triangle {
public:
	Triangle(ID3D11Device* device, ID3D11DeviceContext* devcon);
	~Triangle();

	void Render(ID3D11DeviceContext* devcon);

private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11Buffer* vertexBuffer;

	ID3D11InputLayout* layout;

	VERTEX vertices[3] = {
		VERTEX(-0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		VERTEX( 0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		VERTEX( 0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f)
	};
};

#endif