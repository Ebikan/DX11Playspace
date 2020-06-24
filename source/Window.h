#pragma once
#include "stdafx.h"
#include "Keyboard.h"
#include "MouseCapture.h"
#include "ExceptionBase.h"
#include <optional>
#include <memory>
#include "Graphics.h"

class Window
{
public:
	class Exception : public ExceptionBase
	{
	public:
		Exception(_In_ const char* file, _In_ UINT lineNum, _In_ HRESULT hResult) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(_In_ HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;

	private:
		HRESULT hResult;
	};

private:
	class WindowClass
	{
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
		static unsigned short WindowCount() noexcept;
		static void AddWindowCount() noexcept;
		static void SubWindowCount() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		WindowClass(WindowClass&&) = delete;
		WindowClass& operator=(WindowClass&&) = delete;
		static constexpr const wchar_t* wndClassName = L"Evie's Awesome Window!";
		static WindowClass wndClass;
		HINSTANCE hInst;
		unsigned short instanceCount;
		
	};
public:
	Window(int width, int height, const wchar_t* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;
	void ChangeTitle(_In_ const std::string& str) noexcept;
	// static to process all messages for the WClass.
	static std::optional<int> ProcessMessages() noexcept;
	// graphics class accessor
	Graphics& Gfx() noexcept;
private:
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI HandleMsgUpdate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	HWND hWndSto;
	Keyboard kbd;
	MouseCapture mouse;
	std::unique_ptr<Graphics> pGfx;
};


#define WND_EXEPT( hr ) Window::Exception(__FILE__, __LINE__, hr)