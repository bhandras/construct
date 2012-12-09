#include "construct.h"


namespace Construct
{
	Body::Body()
		: mShape(0)
		, mInvMass(0)
		, mFriction(0)
		, mElasticity(0)
	{ }


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

	}
}
