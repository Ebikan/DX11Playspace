#pragma once
namespace InCap {
	class MouseCapture {
		friend class Window;
	public:
		class Event {
		public:
			enum class Type {
				Invalid,
				MouseLClick,
				MouseRClick,
				MouseMove,
			};

		private:
			POINT loc;
			bool wasRightClick;
			bool wasLeftClick;


		};
		
	public:


	private:
		void setLoc(POINT loc);


		POINT loc;
		bool wasRightClick;
		bool wasLeftClick;
	};
}