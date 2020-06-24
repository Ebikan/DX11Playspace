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
		Event() noexcept : type(Type::Invalid), key(0u) {};
		Event(Type type, char key) noexcept : type(type), key(key) {};
		Event& operator=(const Event&) = delete;
		Event(const Event&) = delete;
	private:
		Type type;
		char key;
	};
	Keyboard() = default;
	bool KeyPressed(char key) noexcept;
	bool KeyDown(char key) const noexcept;
	bool KeyUp(char key) const noexcept;
	char GetChar() noexcept;

private:
	// private but exposed to Window friend.
	// unsigned int because wParam is unsigned int.
	void OnKeyDown(unsigned int key) noexcept;
	void OnKeyUp(unsigned int key) noexcept;
	void OnCharDown(char key) noexcept;
private:
	// Do not direct reference in Window. Handled by Keyboard.
	std::bitset<0xFF> press;
	std::bitset<0xFF> release;
	std::bitset<0xFF> down;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;

};
