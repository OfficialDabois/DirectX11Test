#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <Windows.h>
#include <D3D11.h>
#include <d3dx11.h>
#include <D3DX10.h>
#include <xnamath.h>
#include <vector>

#include "IObject.h"
#include "Camera.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "D3DX10.lib")
#pragma comment(lib, "dxgi.lib")

class Scene {
public:
	Scene(Camera* camera) {};

	void Add(IObject* object);
	void Remove(IObject* object);
	void Render();
private:
	std::vector<IObject*> objects;
};

#endif