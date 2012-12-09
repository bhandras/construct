#include "construct.h"


namespace Construct
{
	Body::Body(Type type)
		: mType(type)
		, mShape(0)
		, mFriction(0)
		, mElasticity(0)
	{ }


	Body::Type Body::getType() const
	{
		return mType;
	}


	void Body::setShape(Shape* shape)
	{
		mShape = shape;
	}


	Shape* Body::getShape()
	{
		return mShape;
	}


	void Body::translate(const Vector2f& translation)
	{
		mPosition += translation;
	}


	void Body::setVelocity(const Vector2f& velocity)
	{
		mVelocity = velocity;
	}


	const Vector2f& Body::getVelocity()
	{
		return mVelocity;
	}


	void Body::update(unsigned int deltaTimeMs)
	{
		if (mShape)
		{
			mShape->transformation().setTranslation(mPosition.x, mPosition.y);
		}
	}
}
