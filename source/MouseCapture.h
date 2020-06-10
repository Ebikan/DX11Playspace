#pragma once
class MouseCapture {
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			Invalid,
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Invalid
		};
		Event();
		Event(Type t, POINT pos);
	private:
		Type type;
		bool leftIsPressed;
		POINT pos;
	};

public:
	MouseCapture() = default;
	MouseCapture(const MouseCapture&) = delete;
	MouseCapture& operator=(const MouseCapture&) = delete;
private: 
	void setLoc(_In_ const POINT loc);
	void OnMouseDown();
	void OnMouseMove();
	void OnMouseScroll();

private:
	POINT loc;
	bool wasRightClick;
	bool wasLeftClick;
};
