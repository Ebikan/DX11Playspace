#include "stdafx.h"
/******************************************************************************

	file:	DebugLayerInfo.cpp

	brief:	none

	date:	6/29/2020 2:43:51 PM

	author:	Evie Brown

	email:	evie@eviebrown.com

******************************************************************************/
#include "Window.h"
#include "Graphics.h"
#include "DebugLayerInfo.h"
#include <memory>
#include <dxgidebug.h>

DebugLayerInfo::DebugLayerInfo() {
	// define what the DXGIGetDebugInterface will look like
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);
	const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr) {
		throw Window::Exception(__FILE__, (UINT)__LINE__, GetLastError());
	}
	// fetch the DXGIGetDebugInterface frow dxgidebug.dll
	VoidCasting_IKnowWhatIAmDoing
	FARPROC debugInterface = (GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"));
	VoidCasting_IKnowWhatIAmDoing
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(debugInterface);
	if (DxgiGetDebugInterface == nullptr)
	{
		throw Window::Exception(__FILE__, (UINT)__LINE__, GetLastError());
	}
	VoidCasting_IKnowWhatIAmDoing
		HRESULT const hr = DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&pInfoQueue));

	if (FAILED(hr)) throw Graphics::HResultException(__LOC__, hr);
}

DebugLayerInfo::~DebugLayerInfo() {
	if (pInfoQueue != nullptr) {
		pInfoQueue->Release();
	}
}

void DebugLayerInfo::Set() noexcept
{
	// set the index (next) so that the next call to GetMessages()
	// will only get errors generated after this call
	next = pInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DebugLayerInfo::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = pInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; i++)
	{
		SIZE_T messageLength = NULL;
		// get the size of message i in bytes
		
		if (FAILED(pInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength))) throw Graphics::HResultException(__LOC__, GetLastError());
		// allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		VoidCasting_IKnowWhatIAmDoing
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// get the message and push its description into the vector
		if (FAILED(pInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength))) throw Graphics::HResultException(__LOC__, GetLastError());
		messages.emplace_back(pMessage->pDescription);
	}
	return messages;

}

