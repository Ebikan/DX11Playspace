#pragma once
/******************************************************************************

	file:	Graphics.h

	brief:	none

	date:	6/21/2020 11:18:01 PM

	author:	Evie Brown

	email:	evie@eviebrown.com

******************************************************************************/
#include <d3d11.h>
#include "ExceptionBase.h"

class Graphics {
public:
	// Standard Exception
	class EStandard : public ExceptionBase {
	public:
		using ExceptionBase::ExceptionBase;
	};
	// HResult Exception
	class EHResult : public ExceptionBase {
	public:
		EHResult(_In_ const char* file, _In_ UINT lineNum, _In_ HRESULT handle) noexcept;
		const char* what() const noexcept override;
		const int lineNum() const noexcept;
		const char* GetType() const noexcept;
	};
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(Graphics&&) = delete;
	~Graphics();
	
	void ClearBuffer(float red, float green, float blue);
	void FrameEnd();

private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTargetView = nullptr;
};


