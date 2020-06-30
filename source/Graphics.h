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
		HResultException(_In_ const char* file, _In_ UINT lineNum, _In_ HRESULT handle, std::vector<std::string> info = {"None"}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		std::string info;
		HRESULT hResult;
	};
	// Device was removed from the binding
	class DeviceRemovedException : public HResultException {
		using HResultException::HResultException;
	};
public:
	Graphics(_In_ HWND hWnd);
	
	void ClearBuffer(float red, float green, float blue);
	void FrameEnd();

private:
#ifdef _DEBUG
	DebugLayerInfo infoManager;
#endif

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView = nullptr;
};


