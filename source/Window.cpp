#include "stdafx.h"

#include <sstream>
#include "WindowsMessageMap.h"
#include <string>
#include "resource.h"


#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;


Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC /*| CS_NOCLOSE*/;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hCursor = LoadCursor(nullptr, IDC_PERSON);
	wc.hbrBackground = nullptr;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::Window(int width, int height, const wchar_t* name) noexcept
{

	this->width = width;
	this->height = height;


	DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX;

	RECT rec;
	rec.left = 0;
	rec.right = rec.left + width;
	rec.top = 0;
	rec.bottom = rec.top + height;

	AdjustWindowRect(&rec, dwStyle, FALSE);

	hWndSto = CreateWindowEx(
		0, WindowClass::GetName(),
		name,
		dwStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rec.right - rec.left,
		rec.bottom - rec.top,
		nullptr, nullptr, WindowClass::GetInstance(),
		this
	);

	ShowWindow(hWndSto, SW_SHOWDEFAULT);
}

Window::~Window() 
{
	DestroyWindow( 0 );
}


LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCT* const createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* const winCls = static_cast<Window*>(createStruct->lpCreateParams);
		//store the window data in the data on the Win32 side
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(winCls));

		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgUpdate));
	
		return winCls->HandleMsg(hWnd, msg, wParam, lParam);
	
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);

}

LRESULT WINAPI Window::HandleMsgUpdate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	static std::wstring title;

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(99);
		break;
	case WM_KEYDOWN:
		if (wParam == 'R')
			SetWindowText(hWnd, L"YO MAMA");
		break;
	case WM_KEYUP:
		if (wParam == 'R')
			SetWindowText(hWnd, L"Window");
		break;
	case WM_CHAR:
		title.push_back((wchar_t)wParam);
		SetWindowText(hWnd, title.c_str());
		break;
	case WM_LBUTTONDOWN:
		POINTS pt = MAKEPOINTS(lParam);
		std::wstringstream ss;
		ss << L"(" << pt.x << L", " << pt.y << L")";
		SetWindowText(hWnd, ss.str().c_str());
		break;

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);

}

Window::Exception::Exception(_In_ const char* file, _In_ unsigned int lineNum, _In_ HRESULT hResult) noexcept
	: ExceptionBase(file, lineNum), hResult(hResult)
{

}

const char* Window::Exception::GetType() const noexcept
{
	return "Window::Exception";
}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< ExceptionBase::what();
	strBuffer = oss.str();
	return strBuffer.c_str();
}


std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hResult;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hResult);
}