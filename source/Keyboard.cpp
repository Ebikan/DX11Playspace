#include "stdafx.h"
#include "Keyboard.h"

// return whether the key was pressed once
bool Keyboard::KeyPressed(char key) {
	return press.test(key);

}
// return whether the key is currently down
bool Keyboard::KeyDown(char key) const {
	return down.test(key);
}
bool Keyboard::KeyUp(char key) const {
	return !down.test(key);
}
// return null if charBuffer is empty.
char Keyboard::GetChar() {
	if (!charBuffer.empty()) {
		char ret = charBuffer.front();
		charBuffer.pop();
		return ret;
	}
	return NULL;
}

void Keyboard::OnKeyDown(unsigned int key) {
	if (!down.test(key))
		press.set();
	else
		press.reset();
	release.reset();
	down.set(key);
}

void Keyboard::OnKeyUp(unsigned int key) {
	if (down.test(key))
		release.set();
	else
		release.reset();
	press.reset(key);
	down.reset(key);
}

void Keyboard::OnCharDown(char key) {
	charBuffer.push(key);
}