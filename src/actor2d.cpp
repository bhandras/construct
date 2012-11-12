#include "construct.h"


Actor2d::Actor2d()
: mDirty(false)
, mPosX(0.0f)
, mPosY(0.0f)
, mRotationRad(0.0f)
, mScaleX(0.0f)
, mScaleY(0.0f)
{ }



void Actor2d::setPosition(float x, float y)
{
	mDirty = true;
	mPosX = x;
	mPosY = y;
}


void Actor2d::setRotationDeg(float angleDeg)
{
	mDirty = true;
	mRotationRad = MathUtil::Numeric::deg2Rad(angleDeg);
}


void Actor2d::setScale(float scaleX, float scaleY)
{
	mDirty = true;
	mScaleX = scaleX;
	mScaleY = scaleY;
}


void Actor2d::update(unsigned int deltaTimeMs)
{
	if (mDirty)
	{
		mTransformation.create(mRotationRad, mPosX, mPosY, 1.0f, 1.0f);
		mDirty = false;
	}
}