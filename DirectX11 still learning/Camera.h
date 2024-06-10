#ifndef CAMERA_H
#define CAMERA_H
#include <iostream>
#include <Windows.h>
#include <D3D11.h>
#include <d3dx11.h>
#include <D3DX10.h>
#include <xnamath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "D3DX10.lib")
#pragma comment(lib, "dxgi.lib")

class Camera {
public:
	Camera(XMVECTOR pos, XMVECTOR target, XMVECTOR up);

	XMMATRIX GetView();
	void Move(XMVECTOR pos);

private:
	XMVECTOR pos;
	XMVECTOR target;
	XMVECTOR up;

	XMMATRIX view;
};

#endif