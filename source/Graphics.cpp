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

//#pragma comment(lib, "d3d11.lib") force linker

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HResultException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HResultException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif



Graphics::Graphics(_In_ HWND hWnd) {


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
	SwapChainDesc.OutputWindow = (HWND)hWnd;
	SwapChainDesc.Windowed = true; //dont force, use IDXGISwapChain::SetFullscreenState
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;   //change to flip model in EvWin BLT 
	SwapChainDesc.Flags = 0;

	HRESULT const hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,	//Device flags, use D3D11_CREATE_DEVICE_FLAGS for debug and 3-2d integrate.
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
		throw Graphics::HResultException(__FILE__, __LINE__, hr);
	}

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
	HRESULT hr;
	if (FAILED(hr = pSwapChain->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw Graphics::DeviceRemovedException(__LOC__, hr);
		}
		else {
			if (FAILED(hr)) throw Graphics::DeviceRemovedException(__LOC__, hr);
		}
	}
}

Graphics::HResultException::HResultException(_In_ const char* file, _In_ UINT lineNum, _In_ HRESULT handle, _In_ std::string) noexcept
	:
	BaseException::BaseException(file, lineNum),
	hResult(handle)
{
		
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
			<< "[Description]: " << error << endl
			<< "[Error Code] 0x" << hex << uppercase << GetErrorCode() << endl
			<< endl;
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

