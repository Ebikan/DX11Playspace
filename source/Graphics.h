#pragma once
/******************************************************************************

	file:	Graphics.h

	brief:	none

	date:	6/21/2020 11:18:01 PM

	author:	Evie Brown

	email:	evie@eviebrown.com

******************************************************************************/
#include <d3d11.h>
#include <wrl.h>
#include "DebugLayerInfo.h"
#include "BaseException.h"

class Graphics {
public:
	// Standard Exception
	class Exception : public BaseException {
	public:
		using BaseException::BaseException;
	};
	// HResult Exception
	class HResultException : public BaseException {
	public:
		HResultException(_In_ const char* file, _In_ UINT lineNum, _In_ HRESULT handle, _In_opt_ std::vector<std::string> info = {"None"}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		std::string info;
		HRESULT hResult;
	};
	class InfoException : public BaseException {
	public:
		InfoException(_In_ const char* file, _In_ UINT lineNum, _In_opt_ std::vector<std::string> infoMsgs = { "No Info" }) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;

	private:
		std::string info;

	};
	// Device was removed from the binding
	class DeviceRemovedException : public HResultException {
	public:
		using HResultException::HResultException;
		const char* GetType() const noexcept override;
	};
public:
	Graphics(_In_ HWND hWnd);
	~Graphics() = default;
	Graphics(_In_ const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(Graphics&&) = delete;

	void ClearBuffer(float red, float green, float blue);
	void FrameEnd();

	void DrawTestTri(float angle, float x = 0.f, float y = 0.f);

private:
#ifdef _DEBUG
	DebugLayerInfo infoManager;
#endif

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};


