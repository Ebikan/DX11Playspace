#include "stdafx.h"
#include "Timer.h"

using namespace std::chrono;

Timer::Timer() noexcept {
	start = steady_clock::now();
	last = start;
}
float Timer::Update() noexcept {
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frametime = last - old;
	return frametime.count();
}
float Timer::SinceLast() const noexcept {
	return duration<float>(steady_clock::now() - last).count();
}

float Timer::PeekStart() const noexcept
{
	return duration<float>(steady_clock::now() - start).count();
}
