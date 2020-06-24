#include "stdafx.h"
#include "Keyboard.h"
#include <cctype>

// return whether the key was pressed once
bool Keyboard::KeyPressed(char key) noexcept {
	return press.test(std::toupper(key));

}
// return whether the key is currently down
bool Keyboard::KeyDown(char key) const noexcept {
	return down.test(std::toupper(key));
}
bool Keyboard::KeyUp(char key) const noexcept {
	return !down.test(std::toupper(key));
}
// return null if charBuffer is empty.
char Keyboard::GetChar() noexcept  {
	if (!charBuffer.empty()) {
		char const ret = charBuffer.front();
		charBuffer.pop();
		return ret;
	}
	return NULL;
}

void Keyboard::OnKeyDown(unsigned int key) noexcept {
	if (!down.test(key))
		press.set();
	else
		press.reset();
	release.reset();
	down.set(key);
}

void Keyboard::OnKeyUp(unsigned int key) noexcept {
	if (down.test(key))
		release.set();
	else
		release.reset();
	press.reset(key);
	down.reset(key);
}

void Keyboard::OnCharDown(char key) noexcept {
	charBuffer.push(key);
}
