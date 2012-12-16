#include "construct.h"


namespace Construct
{
	Body::Body(Type type)
		: mType(type)
		, mShape(0)
		, mInvMass(0.0f)
		, mFriction(0.0f)
		, mElasticity(0.0f)
		, mContactCallback(0)
	{ }


	Body::~Body()
	{
		if (mContactCallback)
		{
			delete mContactCallback;
		}
	}


	void Body::setMass(float mass)
	{
		if (mType == DynamicBody)
		{
			mInvMass = 1.0f / mass;
		}
	}


	float Body::getInvMass() const
	{
		return mInvMass;
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


	const Vector2f& Body::getVelocity() const
	{
		return mVelocity;
	}


	void Body::setElasticity(float elasticity)
	{
		mElasticity = elasticity;
	}


	float Body::getElasticity() const
	{
		return mElasticity;
	}


	void Body::setFriction(float friction)
	{
		mFriction = friction;
	}


	float Body::getFriction() const
	{
		return mFriction;
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


	void Body::contact(Body* body, const Vector2f& pushVector)
	{
		if (mContactCallback)
		{
			mContactCallback->call(body, pushVector);
		}
	}


	void Body::setContactCallback(IFunctor<void(Body*, const Vector2f&)>* callback)
	{
		if (mContactCallback)
		{
			delete mContactCallback;
		}

		mContactCallback = callback;
	}
}
