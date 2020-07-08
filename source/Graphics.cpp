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


//#pragma comment(lib, "d3d11.lib") force linker

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
	SwapChainDesc.OutputWindow = hWnd;
	SwapChainDesc.Windowed = true; //dont force, use IDXGISwapChain::SetFullscreenState
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;   //change to flip model in EvWin BLT 
	SwapChainDesc.Flags = 0;


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
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.f };
#pragma warning(suppress:26485) // Pointer decay not preventable. Sent to API.
	pContext->ClearRenderTargetView(pTargetView.Get(), color);
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

void Graphics::DrawTestTri()
{
	namespace wrl = Microsoft::WRL;

	struct Vertex
	{
		float x;
		float y;
	};

	const Vertex vertices[] = {
		{0.0f, 0.5f},
		{0.5f, -0.5f},
		{-0.5f, -0.5f},
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

	if (FAILED(hr)) throw Graphics::HResultException(__LOC__, hr, infoManager.GetMessages());


	// Bind vertex buffer to pipeline Input Assembler
#ifdef _DEBUG 
	infoManager.Set();
#endif 
	UINT constexpr stride = sizeof(Vertex);
	UINT constexpr offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);
	pContext->Draw(3u, 0u);
#ifdef _DEBUG 
	throw Graphics::DeviceRemovedException(__LOC__, pDevice->GetDeviceRemovedReason(), infoManager.GetMessages());
#else
	throw Graphics::DeviceRemovedException(__LOC__, pDevice->GetDeviceRemovedReason());
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
