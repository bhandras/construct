#include "construct.h"


int Input::mPointerX = 0;
int Input::mPointerY = 0;
bool Input::mKeys[KEY_LAST] = { false };
bool Input::mButtons[BUTTON_LAST] = { false };


bool Input::isKeyDown(Key key)
{
	return mKeys[key];
}


bool Input::isButtonDown(Button button)
{
	return mButtons[button];
}


void Input::getPointer(int& x, int& y)
{
	x = mPointerX;
	y = mPointerY;
}


void Input::setKeyState(Key key, bool isDown)
{
	mKeys[key] = isDown;
}


void Input::setButtonState(Button button, bool isDown)
{
	mButtons[button] = isDown;
}


void Input::setPointerState(int x, int y)
{
	mPointerX = x;
	mPointerY = y;
}
