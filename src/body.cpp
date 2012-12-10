#include "construct.h"


namespace Construct
{
	Body::Body(Type type)
		: mType(type)
		, mShape(0)
		, mFriction(0)
		, mElasticity(0)
		, mContactCallback(0)
	{ }


	Body::~Body()
	{
		if (mContactCallback)
		{
			delete mContactCallback;
		}
	}


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


	void Body::setPosition(float x, float y)
	{
		mPosition.x = x;
		mPosition.y = y;
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
		mPosition += mVelocity;

		if (mShape)
		{
			mShape->transformation().setTranslation(mPosition.x, mPosition.y);
			mShape->update();
		}
	}


	void Body::contact(Body* body)
	{
		if (mContactCallback)
		{
			mContactCallback->call(body);
		}
	}


	void Body::setContactCallback(IFunctor<void(Body*)>* callback)
	{
		if (mContactCallback)
		{
			delete mContactCallback;
		}

		mContactCallback = callback;
	}
}
