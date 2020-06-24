#pragma once
#include "Window.h"
#include "Timer.h"
typedef class Timer Timer;

class App {
public:
	App() noexcept;
	// full cycle ... message loop + master frame
	int Go();
private:
	void DoFrame();
	Window wnd;
	Timer timer;
};