#include "stdafx.h"
/******************************************************************************

	file:	Graphics.cpp

	brief:	none

	date:	6/21/2020 11:18:01 PM

	author:	Evie Brown

	email:	evie@eviebrown.com

******************************************************************************/

#include "Graphics.h"

//#pragma comment(lib, "d3d11.lib") force linker

Graphics::Graphics(HWND hWnd) {


	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferDesc.Height = 0;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = hWnd;
	SwapChainDesc.Windowed = true; //dont force, use IDXGISwapChain::SetFullscreenState
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //change to flip model in Erwin
	SwapChainDesc.Flags = 0;

	/*HRESULT D3DCreateResult =*/ D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		NULL,	//Device flags, use D3D11_CREATE_DEVICE_FLAGS for debug and 3-2d integrate.
		nullptr,
		NULL,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&pSwapChain,
		&pDevice,
		NULL,
		&pContext
	);

	ID3D11Resource* pBackBuffer = nullptr;
	VoidCasting_IKnowWhatIAmDoing
	pSwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	if (pBackBuffer) {
		pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTargetView);
		pBackBuffer->Release();
	}
}

Graphics::~Graphics()
{
	if (pDevice != nullptr) {
		pDevice->Release();
	}
	if (pSwapChain != nullptr) {
		pSwapChain->Release();
	}
	if (pContext != nullptr) {
		pContext->Release();
	}
	if (pTargetView != nullptr) {
		pTargetView->Release();
	}
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.f };
#pragma warning(suppress:26485) // Pointer decay not preventable. Sent to API.
	pContext->ClearRenderTargetView(pTargetView, color);
}

void Graphics::FrameEnd() {
	pSwapChain->Present(1u, 0u);
}