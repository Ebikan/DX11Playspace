#include "stdafx.h"
#include "Window.h"
#include "resource.h"
#include "App.h"

#define MAX_LOADSTRING 50

WCHAR cn[MAX_LOADSTRING];
WCHAR wndName[MAX_LOADSTRING];


int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);
	// load strings
	//LoadString(hInstance, IDS_CLASSNAME, cn, MAX_LOADSTRING);
	//LoadString(hInstance, IDS_TITLE, wndName, MAX_LOADSTRING);

	//create windows

	try
	{	
		App app = App(L"Color Test");
		app.Go();
	}
	catch (const BaseException& e)
	{
		std::string what = e.what();
		MessageBoxA(nullptr, what.c_str(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
		
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;

}