#pragma once
#include <chrono>


class Timer {
public:
	Timer() noexcept;
	float Update() noexcept;
	float SinceLast() const noexcept;
	float PeekStart() const noexcept;

private:
	std::chrono::steady_clock::time_point last;
	std::chrono::steady_clock::time_point start;

};