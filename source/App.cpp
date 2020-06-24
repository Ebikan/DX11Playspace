#include "stdafx.h"
#include "Timer.h"
#include <iomanip>
#include <sstream>
#include "App.h"
#include "Graphics.h"
#include "Window.h"

App::App() noexcept : wnd(1200, 950, L"Color Test") {}

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

	wnd.Gfx().ClearBuffer(fabsf(sinf(elapsedTime)), fabsf(sinf(elapsedTime/1.1f+0.19635f)), fabsf(sinf(elapsedTime/1.3f + 0.26635f)));
	wnd.Gfx().FrameEnd();
}