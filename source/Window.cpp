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

	wc.cbSize = sizeof(wc);
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

	instanceCount = 0;
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(wndClassName, GetInstance());
}

const wchar_t* Window::WindowClass::GetName() noexcept {
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
	return wndClass.hInst;
}

unsigned short Window::WindowClass::WindowCount() noexcept {
	return wndClass.instanceCount;
}

void Window::WindowClass::AddWindowCount() noexcept {
	++wndClass.instanceCount;
}

void Window::WindowClass::SubWindowCount() noexcept {
	--wndClass.instanceCount;
}


Window::Window(int width, int height, const wchar_t* name) {

	this->width = width;
	this->height = height;


	DWORD constexpr dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX;

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
	WindowClass::AddWindowCount();

	// Create Graphics
	pGfx = std::make_unique<Graphics>( hWndSto );

}

Window::~Window() {
	DestroyWindow(hWndSto);
}

void Window::ChangeTitle(_In_ const std::string& str) noexcept {
	if (GetFocus() == hWndSto)
		SetWindowTextA(hWndSto, str.c_str());
}

int Window::Width() const noexcept
{
	return width;
}

int Window::Height() const noexcept
{
	return height;
}

// Message Pump
std::optional<int> Window::ProcessMessages() noexcept {
	// Run messages till out.
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return static_cast<int>(msg.wParam);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

Graphics& Window::Gfx()
{

	return *pGfx;
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		VoidCasting_IKnowWhatIAmDoing
		const CREATESTRUCT* const createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		
		Window* const winCls = static_cast<Window*>(createStruct->lpCreateParams);
		if (!winCls) {
			PostQuitMessage(1);
			return NULL;
		}

		//store the window data in the data on the Win32 side
		VoidCasting_IKnowWhatIAmDoing
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(winCls));

		VoidCasting_IKnowWhatIAmDoing
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgUpdate));

		return winCls->HandleMsg(hWnd, msg, wParam, lParam);

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);

}

LRESULT WINAPI Window::HandleMsgUpdate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	VoidCasting_IKnowWhatIAmDoing
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (!pWnd) {
		PostQuitMessage(2);
		return NULL;
	}
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);

}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	static WindowsMessageMap mm;
	//OutputDebugStringA(mm(msg, lParam, wParam).c_str());



	static std::wstring title;
	std::stringstream out;
	POINTS pt = {};


	switch (msg)
	{

		/* ------- Start Window Messages -------- */

	case WM_CLOSE:
		WindowClass::SubWindowCount();
		if (!WindowClass::WindowCount())
			PostQuitMessage(69);
		break;

		/* ------- End Window Messages -------- */


		/* ------- Start Mouse Messages -------- */

	case WM_MOUSEMOVE:
		pt = MAKEPOINTS(lParam);

		// if inside client
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow()) {
				SetCapture(hWnd);
				mouse.OnMouseEnter(pt.x, pt.y);
			}
		}
		// else outside client
		else {
			// keep click drag info.
			if (mouse.LeftIsPressed() || mouse.RightIsPressed()) {
				mouse.OnMouseMove(pt.x, pt.y);
			}
			// no longer click draging
			else {
				ReleaseCapture();
				mouse.OnMouseLeave(pt.x, pt.y);
			}

		}
		//out << pt.y << std::endl;
		//OutputDebugStringA(out.str().c_str());
		break;
	case WM_LBUTTONDOWN:
		pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		SetForegroundWindow(hWnd);
		break;
	case WM_RBUTTONDOWN:
		pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	case WM_LBUTTONUP:
		pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave(pt.x, pt.y);
		}
		break;
	case WM_RBUTTONUP:
		pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave(pt.x, pt.y);
		}
		break;
	case WM_MOUSEWHEEL:
	{
		short const delta = GET_WHEEL_DELTA_WPARAM(wParam);
		pt = MAKEPOINTS(lParam);
		if (delta > 0)
			mouse.OnWheelUp(pt.x, pt.y);
		break;
	}

		/* ------- End Mouse Messages -------- */



		/* ------- Start Keyboard Messages -------- */

	case WM_KEYDOWN:
		kbd.OnKeyDown(static_cast<unsigned int>(wParam));
		ChangeTitle(kbd.KeyDown('g') ? "G" : "F");
		break;
	case WM_KEYUP:
		kbd.OnKeyUp(static_cast<unsigned int>(wParam));
		break;
	case WM_CHAR:
		kbd.OnCharDown(static_cast<char>(wParam));
		break;

		/* ------- End Keyboard Messages -------- */

	} // msg switch end

	return DefWindowProc(hWnd, msg, wParam, lParam);

}




Window::Exception::Exception(_In_ const char* file, _In_ unsigned int lineNum, _In_ HRESULT hResult) noexcept
	: BaseException(file, lineNum), hResult(hResult)
{

}

const char* Window::Exception::GetType() const noexcept
{
	return "Window::Exception";
}

const char* Window::Exception::what() const noexcept
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


std::string Window::Exception::TranslateErrorCode(_In_ HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	DWORD const nMsgLen = FormatMessageA(
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