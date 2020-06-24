#include "stdafx.h"
#include "MouseCapture.h"
#include <queue>

std::pair<int, int> MouseCapture::GetPos() const noexcept {
	return std::pair<int, int>(x,y);
}

int MouseCapture::GetPosX() const noexcept {
	return x;
}
int MouseCapture::GetPosY() const noexcept {
	return y;
}
bool MouseCapture::LeftIsPressed() const noexcept {
	return leftIsPressed;
}
bool MouseCapture::RightIsPressed() const noexcept {
	return rightIsPressed;
}
bool MouseCapture::IsInWindow() const noexcept {
	return isInWindow;
}

MouseCapture::Event MouseCapture::Read() noexcept
{ 
	if (!buffer.empty()) {
		Event const readEvent = buffer.front();
		buffer.pop();
	}
	return Event();
}

void MouseCapture::Flush() noexcept
{
	while (!buffer.empty())
		buffer.pop();
}

void MouseCapture::OnMouseMove(int newx, int newy) noexcept
{
	x = newx;
	y = newy;

	buffer.push(Event(Event::Type::Move, *this));
	TrimBuffer();
}

void MouseCapture::OnLeftPressed(int newx, int newy) noexcept
{
	leftIsPressed = true;
	
	UNREFERENCED_PARAMETER(newx);
	UNREFERENCED_PARAMETER(newy);

	buffer.push(Event(Event::Type::LPress, *this));
	TrimBuffer();
}

void MouseCapture::OnLeftReleased(int newx, int newy) noexcept
{
	leftIsPressed = false;

	UNREFERENCED_PARAMETER(newx);
	UNREFERENCED_PARAMETER(newy);
	
	buffer.push(Event(Event::Type::LRelease, *this));
	TrimBuffer();
}

void MouseCapture::OnRightPressed(int newx, int newy) noexcept
{
	rightIsPressed = true;

	UNREFERENCED_PARAMETER(newx);
	UNREFERENCED_PARAMETER(newy);

	buffer.push(Event(Event::Type::RPress, *this));
	TrimBuffer();
}

void MouseCapture::OnRightReleased(int newx, int newy) noexcept
{
	rightIsPressed = false;

	UNREFERENCED_PARAMETER(newx);
	UNREFERENCED_PARAMETER(newy);

	buffer.push(Event(Event::Type::RRelease, *this));
	TrimBuffer();
}

void MouseCapture::OnWheelUp(int newx, int newy) noexcept
{
	UNREFERENCED_PARAMETER(newx);
	UNREFERENCED_PARAMETER(newy);

	buffer.push(Event(Event::Type::WheelUp, *this));
	TrimBuffer();
}

void MouseCapture::OnWheelDown(int newx, int newy) noexcept
{
	UNREFERENCED_PARAMETER(newx);
	UNREFERENCED_PARAMETER(newy);

	buffer.push(Event(Event::Type::WheelDown, *this));
	TrimBuffer();
}

void MouseCapture::OnMouseEnter(int newx, int newy) noexcept {

	isInWindow = true;

	UNREFERENCED_PARAMETER(newx);
	UNREFERENCED_PARAMETER(newy);

	buffer.push(Event(Event::Type::Enter, *this));
	TrimBuffer();
}

void MouseCapture::OnMouseLeave(int newx, int newy) noexcept {

	isInWindow = false;

	UNREFERENCED_PARAMETER(newx);
	UNREFERENCED_PARAMETER(newy);

	buffer.push(Event(Event::Type::Leave, *this));
	TrimBuffer();
}


void MouseCapture::TrimBuffer() noexcept
{
	if (buffer.size() > bufferSize) {
		buffer.pop();
	}
}

