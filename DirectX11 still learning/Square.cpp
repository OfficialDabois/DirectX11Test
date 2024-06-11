#include "Square.h"

Square::Square(ID3D11Device* dev, ID3D11DeviceContext* devcon, XMMATRIX view, XMVECTOR position): devcon(devcon), view(view) {
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	bd.ByteWidth = sizeof(VERTEX) * 4;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vInit;
	vInit.pSysMem = vertices;

	dev->CreateBuffer(&bd, &vInit, &vertexBuffer);

	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	bd.ByteWidth = sizeof(UINT) * 2 * 3;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA iInit;
	iInit.pSysMem = indices;

	dev->CreateBuffer(&bd, &iInit, &indexBuffer);

	ID3D10Blob* VS, * PS;
	D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "VS", "vs_5_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(L"shaders.hlsl", 0, 0, "PS", "ps_5_0", 0, 0, 0, &PS, 0, 0);

	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), 0, &vertexShader);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), 0, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &inputLayout);

	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(D3D11_BUFFER_DESC));

	cbd.ByteWidth = sizeof(cbPerObj);
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	dev->CreateBuffer(&cbd, NULL, &cbPerObjBuffer);
	
	world = XMMatrixIdentity() * XMMatrixTranslation(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position));
	proj = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)800 / (float)600, 0.1f, 1000.0f);

	cbPerObject.WVP = XMMatrixTranspose(world * view * proj);

	devcon->UpdateSubresource(cbPerObjBuffer, 0, NULL, &cbPerObject, 0, 0);
}

Square::Square(Square& squ): devcon(squ.devcon) {
	vertexShader = squ.vertexShader;
	pixelShader = squ.pixelShader;
	vertexBuffer = squ.vertexBuffer;
	indexBuffer = squ.indexBuffer;
	cbPerObjBuffer = squ.cbPerObjBuffer;
	cbPerObject = squ.cbPerObject;
	inputLayout = squ.inputLayout;
	world = squ.world;
	view = squ.view;
	proj = squ.proj;
}

Square& Square::operator=(const Square& squ) {
	return *this;
}

void Square::Update(XMMATRIX view) {
	this->view = view;
	cbPerObject.WVP = XMMatrixTranspose(world * view * proj);

	devcon->UpdateSubresource(cbPerObjBuffer, 0, NULL, &cbPerObject, 0, 0);
}

void Square::Move(XMVECTOR position) {
	world = XMMatrixIdentity() * XMMatrixTranslation(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position));
}

void Square::Render() {
	devcon->VSSetShader(vertexShader, 0, 0);
	devcon->PSSetShader(pixelShader, 0, 0);

	devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->IASetInputLayout(inputLayout);
	devcon->VSSetConstantBuffers(0, 1, &cbPerObjBuffer);

	devcon->DrawIndexed(6, 0, 0);
}

Square::~Square() {
	vertexBuffer->Release();
	indexBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();
}