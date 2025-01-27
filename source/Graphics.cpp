#include "stdafx.h"
/******************************************************************************

	file:	Graphics.cpp

	brief:	none

	date:	6/21/2020 11:18:01 PM

	author:	Evie Brown

	email:	evie@eviebrown.com

******************************************************************************/

#include "Graphics.h"
#include <sstream>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXMath.h>

//#pragma comment(lib, "D3DCompiler.lib")
//#pragma comment(lib, "d3d11.lib") force linker


Graphics::Graphics(_In_ HWND hWnd) {


	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = 0u;
	SwapChainDesc.BufferDesc.Height = 0u;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0u;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0u;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.SampleDesc.Count = 8u;
	SwapChainDesc.SampleDesc.Quality = static_cast<UINT>(D3D11_STANDARD_MULTISAMPLE_PATTERN);
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1u;
	SwapChainDesc.OutputWindow = hWnd;
	SwapChainDesc.Windowed = true; //dont force, use IDXGISwapChain::SetFullscreenState
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;   //change to flip model in EvWin BLT 
	SwapChainDesc.Flags = 0u;


#ifdef _DEBUG
	infoManager.Set();
#endif // _DEBUG

	HRESULT const hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
#ifdef _DEBUG
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
#else // _DEBUG
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,	//Device flags, use D3D11_CREATE_DEVICE_FLAGS for debug and 3-2d integrate.
#endif
		nullptr,
		NULL,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	);
	if (FAILED(hr)) {
#ifdef _DEBUG
		throw Graphics::HResultException(__FILE__, __LINE__, hr, infoManager.GetMessages());
#else
		throw Graphics::HResultException(__FILE__, __LINE__, hr);
#endif // _DEBUG
	}

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), &pBackBuffer);
	if (pBackBuffer) {
		pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTargetView);
	}

	// Create Depth Buffer
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	pDevice->CreateDepthStencilState(&dsDesc, &pDSState);

	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC depthDesc = {};
	DXGI_SWAP_CHAIN_DESC scDesc;
	pSwapChain->GetDesc(&scDesc);
	depthDesc.Width = scDesc.BufferDesc.Width;
	depthDesc.Height = scDesc.BufferDesc.Height;
	depthDesc.MipLevels = 1u;
	depthDesc.ArraySize = 1u;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count = 8u;
	depthDesc.SampleDesc.Quality = static_cast<UINT>(D3D11_STANDARD_MULTISAMPLE_PATTERN);
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil);

	// create a view of the depth buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvDesc.Texture2D.MipSlice = 0u;

	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &pDSV);

	// Set Render Target
	pContext->OMSetRenderTargets(1u, pTargetView.GetAddressOf(), pDSV.Get());

	if (FAILED(hr)) {
#ifdef _DEBUG
		throw Graphics::HResultException(__FILE__, __LINE__, hr, infoManager.GetMessages());
#else
		throw Graphics::HResultException(__FILE__, __LINE__, hr);
#endif // _DEBUG
	}
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.f };
#pragma warning(suppress:26485) // Pointer decay not preventable. Sent to API.
	pContext->ClearRenderTargetView(pTargetView.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.f, 0u);
}

void Graphics::FrameEnd() {
	HRESULT hr;
#ifdef _DEBUG 
	infoManager.Set();

#endif 
	if (FAILED(hr = pSwapChain->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
#ifdef _DEBUG 
			throw Graphics::DeviceRemovedException(__LOC__, pDevice->GetDeviceRemovedReason(), infoManager.GetMessages());
#else
			throw Graphics::DeviceRemovedException(__LOC__, pDevice->GetDeviceRemovedReason());
#endif
		}
		else {
#ifdef _DEBUG 
			if (FAILED(hr)) throw Graphics::HResultException(__LOC__, hr, infoManager.GetMessages());
#else
			if (FAILED(hr)) throw Graphics::HResultException(__LOC__, hr);
#endif
		}
	}
}

void Graphics::DrawTestTri(float angle, float x, float y)
{
	namespace wrl = Microsoft::WRL;
	namespace dx = DirectX;

	DXGI_SWAP_CHAIN_DESC scDesc;
	pSwapChain->GetDesc(&scDesc);

	struct Vertex
	{
		struct Pos {
			float x;
			float y;
			float z;
		} pos;
	};

	const Vertex vertices[] = {


		{ -1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,-1.0f},
		{ -1.0f,1.0f,-1.0f},
		{ 1.0f,1.0f,-1.0f},
		{ -1.0f,-1.0f,1.0f},
		{ 1.0f,-1.0f,1.0f},
		{ -1.0f,1.0f,1.0f},
		{ 1.0f,1.0f,1.0f},

	};

	const unsigned short vertFaces[] = {
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4


	};

	D3D11_BUFFER_DESC desc = {
		sizeof(vertices),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER,
		NULL,
		NULL,
		sizeof(Vertex),
	};
	D3D11_SUBRESOURCE_DATA const srd = {
		vertices,
		NULL,
		NULL,
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	HRESULT const hr = pDevice->CreateBuffer(&desc, &srd, &pVertexBuffer);
#ifdef _DEBUG
	if (FAILED(hr)) throw Graphics::HResultException(__LOC__, hr, infoManager.GetMessages());
#endif // _DEBUG


#ifdef _DEBUG 
	infoManager.Set();
#endif 
	// Bind vertex buffer to pipeline Input Assembler
	UINT constexpr stride = sizeof(Vertex);
	UINT constexpr offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


	wrl::ComPtr<ID3D11Buffer> pVertIndexBuffer;
	D3D11_BUFFER_DESC vertIndexDesc = {
		sizeof(vertFaces),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER,
		NULL,
		NULL,
		sizeof(unsigned short),
	};
	D3D11_SUBRESOURCE_DATA indSrD = {};
	indSrD.pSysMem = vertFaces;
	pDevice->CreateBuffer(&vertIndexDesc, &indSrD, &pVertIndexBuffer);
	pContext->IASetIndexBuffer(pVertIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	struct ConstantBuffer {
		dx::XMMATRIX transform;
	};


	const ConstantBuffer cb =
	{
		{
		dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ(angle) *
			dx::XMMatrixRotationY(angle) *
			dx::XMMatrixTranslation(x,y,4.f) *
			dx::XMMatrixPerspectiveLH(1.f, static_cast<float> (scDesc.BufferDesc.Height)/ static_cast<float> (scDesc.BufferDesc.Width), 0.5f, 10.f)
			
		)
		}
	};


	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = sizeof(cb);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = NULL;
	cbDesc.StructureByteStride = 0u;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA pCBSubRes = {};
	pCBSubRes.pSysMem = &cb;

	wrl::ComPtr<ID3D11Buffer>pConstBuffer;
	pDevice->CreateBuffer(&cbDesc, &pCBSubRes, &pConstBuffer);
	pContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());


	struct ConstantBuffer2 {
		struct {
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{50.f, 56.f, 168.f, 255.f},
			{150.f, 0.f, 0.f, 255.f},
			{174, 255, 23, 255.f},
			{43, 167, 186, 255.f},
			{121, 136, 201, 255.f},
			{129.f, 50.f, 168.f, 255.f},
		}
	};

	D3D11_BUFFER_DESC cbDesc2;
	cbDesc2.ByteWidth = sizeof(cb2);
	cbDesc2.Usage = D3D11_USAGE_DEFAULT;
	cbDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc2.CPUAccessFlags = 0u;
	cbDesc2.MiscFlags = 0u;
	cbDesc2.StructureByteStride = 0u;

	wrl::ComPtr<ID3D11Buffer> cb2Buff;

	D3D11_SUBRESOURCE_DATA cbSub2 = {};
	cbSub2.pSysMem = &cb2;

	pDevice->CreateBuffer(&cbDesc2, &cbSub2, &cb2Buff);

	pContext->PSSetConstantBuffers(0u, 1u, cb2Buff.GetAddressOf());






	wrl::ComPtr<ID3DBlob> pBlob;
	// Create Pixel Shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	// Bind Pixel Shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// Create Vertex Shader
	wrl::ComPtr<ID3D11VertexShader> pVertShader;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertShader);
	// Bind Vertex Shader
	pContext->VSSetShader(pVertShader.Get(), nullptr, 0u);

	// Input layout for vertex shader for 2d
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ieDesc[] = {
		{"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
	};
	pDevice->CreateInputLayout(ieDesc, static_cast<UINT>(std::size(ieDesc)), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
	pContext->IASetInputLayout(pInputLayout.Get());





	// Set Viewport

	D3D11_VIEWPORT view;
	view.TopLeftX = 0;
	view.TopLeftY = 0;
	view.Width = static_cast<float> (scDesc.BufferDesc.Width);
	view.Height = static_cast<float> (scDesc.BufferDesc.Height);
	view.MinDepth = 0;
	view.MaxDepth = 1;
	pContext->RSSetViewports(1u, &view);

	// Set Primitive Topology
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// Draw
	pContext->DrawIndexed(static_cast<UINT> (std::size(vertFaces)), 0u, 0u);
#ifdef _DEBUG 
	if (FAILED(pDevice->GetDeviceRemovedReason())) throw Graphics::DeviceRemovedException(__LOC__, pDevice->GetDeviceRemovedReason(), infoManager.GetMessages());
#else
	if (pDevice->GetDeviceRemovedReason()) throw Graphics::DeviceRemovedException(__LOC__, pDevice->GetDeviceRemovedReason());
#endif


}

Graphics::HResultException::HResultException(_In_ const char* file, _In_ UINT lineNum, _In_ HRESULT handle, _In_opt_ std::vector<std::string> msgVec) noexcept
	:
	BaseException::BaseException(file, lineNum),
	hResult(handle)
{

#ifdef _DEBUG


	// join all info messages with newlines into single string
	for (const std::string& m : msgVec)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
#endif // _DEBUG

}

const char* Graphics::HResultException::what() const noexcept
{

	LPSTR error = nullptr;

	FormatMessageA(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM
		// allocate buffer on local heap for error text
		| FORMAT_MESSAGE_ALLOCATE_BUFFER
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		GetErrorCode(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&error,  // output 
		0, // minimum size for output buffer
		nullptr);   // arguments - see note 

	if (error)
	{
		using namespace std;
		std::stringstream buffer;
		buffer
			<< GetOriginString() << endl
			<< "[Description] " << error << endl
			<< "[Error Code] 0x" << hex << uppercase << GetErrorCode() << endl
			<< "[Info] " << info << endl;
		LocalFree(error);
		error = nullptr;
		strBuffer = buffer.str();
		return strBuffer.c_str();
	}
	return "";
}

const char* Graphics::HResultException::GetType() const noexcept
{
	using namespace std;
	stringstream ss;
	ss << "HRESULT Graphics Exception" << endl;

	strBuffer = ss.str();
	return strBuffer.c_str();
}

HRESULT Graphics::HResultException::GetErrorCode() const noexcept
{
	return hResult;
}

std::string Graphics::HResultException::GetErrorString() const noexcept
{
	return std::string();
}

std::string Graphics::HResultException::GetErrorDescription() const noexcept
{
	return std::string();
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Device Removed Exception";
}

Graphics::InfoException::InfoException(_In_ const char* file, _In_ UINT lineNum, _In_opt_ std::vector<std::string> infoMsgs) noexcept
	:
	BaseException(file, lineNum)
{
	for (const auto& m : infoMsgs) {
		info += m;
		info.push_back('\n');
	}
	// final line remove.
	if (!info.empty()) {
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	using namespace std;
	std::stringstream buffer;
	buffer
		<< GetOriginString() << endl
		<< "[Error Info] " << GetErrorInfo() << endl;
	strBuffer = buffer.str();
	return strBuffer.c_str();

}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}
