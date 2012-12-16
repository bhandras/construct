#pragma once


namespace Construct
{
	class Input
	{
	public:
		enum Key
		{
			K_LEFT,
			K_RIGHT,
			K_UP,
			K_DOWN,
			K_RETURN,

			KEY_LAST
		};

		enum Button
		{
			B_LEFT,
			B_RIGHT,

			BUTTON_LAST
		};

		static bool isKeyDown(Key key);

		static bool isButtonDown(Button button);
		static void getPointer(int& x, int& y);

		static void setKeyState(Key key, bool isDown);
		static void setButtonState(Button button, bool isDown);
		static void setPointerState(int x, int y);

	private:
		static int mPointerX;
		static int mPointerY;
		static bool mKeys[KEY_LAST];
		static bool mButtons[BUTTON_LAST];
	};
}
