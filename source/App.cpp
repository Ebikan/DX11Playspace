#include "stdafx.h"
#include "Timer.h"
#include <iomanip>
#include <sstream>
#include "App.h"
#include "Graphics.h"
#include "Window.h"


App::App(const wchar_t* name) : wnd(1400, 1200, name) {}

int App::Go() {
	while (true) {
		if (const auto ecode = Window::ProcessMessages()) {
			return ecode.value();
		}
		DoFrame();
	}
}


void App::DoFrame() {

	float const elapsedTime = timer.PeekStart()/2.f;
	// Clear Screen
	wnd.Gfx().ClearBuffer(fabsf(sinf(elapsedTime)/1.5f), fabsf(sinf(elapsedTime/1.1f+0.19635f)), fabsf(sinf(elapsedTime/1.3f + 0.26635f)));
	// Frame Operations

	wnd.Gfx().DrawTestTri(elapsedTime, static_cast<float>(wnd.mouse.GetPosX()) / static_cast<float>(wnd.Width()) , - static_cast<float>(wnd.mouse.GetPosY()) / static_cast<float>(wnd.Height()));

	// End Frame
	wnd.Gfx().FrameEnd();
}