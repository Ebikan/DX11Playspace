#include "stdafx.h"
#include "Timer.h"
#include <iomanip>
#include <sstream>
#include "App.h"

App::App() : wnd(1200, 950, L"AppWindow.woot") {}

int App::Go() {
	while (true) {
		if (const auto ecode = Window::ProcessMessages()) {
			return ecode.value();
		}
		DoFrame();
	}
}

void App::DoFrame() {

	const float time = timer.SinceLast();
	std::ostringstream oss;
	oss << "Time Elapsed: " << std::setprecision(1) << std::fixed << time << "s";
	//wnd.ChangeTitle(oss.str());
}