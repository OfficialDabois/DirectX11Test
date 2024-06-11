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

#include "Camera.h"
#include "Types.h"
#include "Triangle.h"
#include "Square.h"

#define width 800
#define height 600

void InitD3D(HWND hWnd);
void GetCurrentAdapter(IDXGIAdapter1** pAdapter);
void SetScene(void);
void UpdateScene(void);
void RenderFrame(void);
void CleanD3D(void);

IDXGIFactory1* pFactory;
IDXGIAdapter1* pAdapter;

IDXGISwapChain* swapChain;
ID3D11Device* dev;
ID3D11DeviceContext* devcon;

ID3D11RenderTargetView* renderTargetView;

ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthBuffer;

std::unique_ptr<Camera> camera;
Square* square1;

float timer = 0.001;

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPSTR lpCmdLine,
	int nShowCmd) {
	HWND hWnd;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"WindowClass";
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"Back at it bois",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nShowCmd);

	MSG msg;

	InitD3D(hWnd);

	while (TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			UpdateScene();
			RenderFrame();
		}
	}

	CleanD3D();

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam) {
	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void InitD3D(HWND hWnd) {
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory);

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Windowed = TRUE;

	GetCurrentAdapter(&pAdapter);

	D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &dev, NULL, &devcon);

	pFactory->CreateSwapChain(dev, &scd, &swapChain);

	ID3D11Texture2D* bBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&bBuffer);

	dev->CreateRenderTargetView(bBuffer, NULL, &renderTargetView);
	
	D3D11_TEXTURE2D_DESC dbtd;
	ZeroMemory(&dbtd, sizeof(D3D11_TEXTURE2D_DESC));

	dbtd.ArraySize = 1;
	dbtd.Width = width;
	dbtd.Height = height;
	dbtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dbtd.MipLevels = 1;
	dbtd.SampleDesc.Count = 1;
	dbtd.SampleDesc.Quality = 0;
	dbtd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dbtd.Usage = D3D11_USAGE_DEFAULT;

	dev->CreateTexture2D(&dbtd, NULL, &depthBuffer);
	dev->CreateDepthStencilView(depthBuffer, NULL, &depthStencilView);

	devcon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));

	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	devcon->RSSetViewports(1, &vp);

	SetScene();
}

void GetCurrentAdapter(IDXGIAdapter1** pAdapter) {
	for (UINT i = 0; pFactory->EnumAdapters1(i, pAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
		DXGI_ADAPTER_DESC1 adaptDesc = DXGI_ADAPTER_DESC1();
		(*pAdapter)->GetDesc1(&adaptDesc);
		std::wstring outStr = L"Using first adaptor found. Current info:\n";
		outStr.append(adaptDesc.Description);
		LPCWSTR finStr = outStr.c_str();
		

		MessageBox(NULL, finStr, L"Adapter", MB_OK);
		break;
	}
}

void SetScene(void) {
	XMVECTOR position = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	XMVECTOR target = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	camera = std::unique_ptr<Camera>(new Camera(position, target, up));

	square1 = new Square(dev, devcon, camera->GetView(), XMVectorSet(0.5f, 0.5f, 0.0f, 0.0f));
}

void UpdateScene(void) {

}

void RenderFrame(void) {
	float colour[4] = { 1.0f, 0.3f, 0.5f, 1.0f };
	devcon->ClearRenderTargetView(renderTargetView, colour);
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	square1->Render();

	swapChain->Present(0, 0);
}

void CleanD3D(void) {
	delete square1;
	swapChain->Release();
	depthBuffer->Release();
	depthStencilView->Release();
	dev->Release();
	devcon->Release();
	renderTargetView->Release();
	pAdapter->Release();
	pFactory->Release();
}