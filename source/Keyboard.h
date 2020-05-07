#pragma once
#include <bitset>
#include <queue>


class Keyboard {
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			Invalid,
			Pressed,
			Released
		};
		Event() : type(Type::Invalid), key(0u) {};
		Event(Type type, char key) : type(type), key(key) {};
		Event& operator=(const Event&) = delete;
		Event(const Event&) = delete;
	private:
		Type type;
		char key;
	};
	bool KeyPressed(char key);
	bool KeyDown(char key) const;
	bool KeyUp(char key) const;
	char GetChar();

private:
	// private but exposed to Window friend.
	// unsigned int because wParam is unsigned int.
	void OnKeyDown(unsigned int key);
	void OnKeyUp(unsigned int key);
	void OnCharDown(char key);
private:
	// Do not direct reference in Window. Handled by Keyboard.
	std::bitset<0xFF> press;
	std::bitset<0xFF> release;
	std::bitset<0xFF> down;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;

};