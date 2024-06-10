#include "Triangle.h"

Triangle::Triangle(ID3D11Device* dev, ID3D11DeviceContext* devcon) {
		ID3D10Blob* VS, * PS;
		D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "VS", "vs_5_0", 0, 0, 0, &VS, 0, 0);
		D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "PS", "ps_5_0", 0, 0, 0, &PS, 0, 0);

		dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &vertexShader);
		dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pixelShader);

		D3D11_INPUT_ELEMENT_DESC ied[] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &layout);

		D3D11_BUFFER_DESC vbd;
		ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));

		vbd.ByteWidth = sizeof(VERTEX) * 3;
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA ms;
		ZeroMemory(&ms, sizeof(D3D11_SUBRESOURCE_DATA));

		ms.pSysMem = vertices;
		dev->CreateBuffer(&vbd, &ms, &vertexBuffer);
}

void Triangle::Render(ID3D11DeviceContext* devcon) {
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	devcon->VSSetShader(vertexShader, 0, 0);
	devcon->PSSetShader(pixelShader, 0, 0);

	devcon->IASetInputLayout(layout);
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->Draw(3, 0);
}

Triangle::~Triangle() {
	vertexShader->Release();
	pixelShader->Release();
	vertexBuffer->Release();
	layout->Release();
}