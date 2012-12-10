#include "construct.h"


namespace Construct
{
	Space::~Space()
	{
		for (size_t i = 0; i < mBodies.size(); ++i)
		{
			delete mBodies[i];
		}
	}


	void Space::setGravity(const Vector2f& gravity)
	{
		mGravity = gravity;
	}


	Body* Space::addBody(Body::Type bodyType)
	{
		Body* body = new Body(bodyType);
		mBodies.push_back(body);
		return body;
	}


	void Space::update(unsigned int deltaTimeMs)
	{
		for (size_t i = 0; i < mBodies.size(); ++i)
		{
			if (mBodies[i]->getType() != Body::StaticBody)
			{
				Vector2f v = mBodies[i]->getVelocity() + mGravity;
				mBodies[i]->setVelocity(v);
			}

			mBodies[i]->update(deltaTimeMs);
		}


		for (size_t i = 0; i < mBodies.size(); ++i)
		{
			Body* body1 = mBodies[i];
			Shape* shape1 = body1->getShape();

			for (size_t j = i + 1; j < mBodies.size(); ++j)
			{
				Body* body2 = mBodies[j];
				
				if (body1->getType() == Body::StaticBody && body2->getType() == Body::StaticBody)
				{
					// static bodies do not collide
					continue;
				}
				
				Shape* shape2 = body2->getShape();

				if (shape1 && shape2)
				{
					// polygon - polygon collision
					if (shape1->getType() == Shape::PolygonShape && shape2->getType() == Shape::PolygonShape)
					{
						Polygon* p1 = static_cast<Polygon*>(shape1);
						Polygon* p2 = static_cast<Polygon*>(shape2);
						Vector2f minSeparation;

						if (Collision2d::intersectPolygons(p1->edges(), p1->transformation().translation(), p2->edges(), p2->transformation().translation(), minSeparation))
						{
							handleCollision(body1, body2, minSeparation);							
						}
					}
				}
			}
		}
	}


	void Space::handleCollision(Body* body1, Body* body2, const Vector2f& minSeparation)
	{
		Vector2f n = minSeparation;
		n.normalize();

		// clamp sum friction and elasticity
		float friction = std::min(body1->getFriction() + body2->getFriction(), 1.0f);
		float elasticity = std::min(body1->getElasticity() + body2->getElasticity(), 1.0f);

		if (body1->getType() == Body::StaticBody)
		{
			Vector2f v = calculateVelocity(body2->getVelocity(), n, elasticity, friction);

			body2->setVelocity(v);
			body2->translate(minSeparation * -1.0f);
			body2->contact(body1, minSeparation * -1.0f);
		}
		else if (body2->getType() == Body::StaticBody)
		{
			Vector2f v = calculateVelocity(body1->getVelocity(), n, elasticity, friction);

			body1->setVelocity(v);
			body1->translate(minSeparation);
			body1->contact(body2, minSeparation);
		}
		else
		{
			Vector2f v = calculateVelocity(body1->getVelocity() - body2->getVelocity(), n, elasticity + 1.0f, friction + 1.0f);
		
			body1->setVelocity(v * 0.5f);
			body1->translate(minSeparation * 0.5f);
			body1->contact(body2, minSeparation * 0.5f);

			body2->setVelocity(v * -0.5f);
			body2->translate(minSeparation * -0.5f);
			body2->contact(body1, minSeparation * -0.5f);
		}
	}


	Vector2f Space::calculateVelocity(const Vector2f& v, const Vector2f& n, float elasticity, float friction) const
	{
		/*

		V: velocity of the body
		N: normalized separation vector

		// velocity projected to separation
		Vn = (V dot N) * N;

		// horizontal component of Vn
		Vt = V – Vn;

		// the new velocity, where friction is [0,1] elasticity is [0,1] 
		V' = Vt * (1 – friction) + Vn  * -(elasticity);

		*/

		Vector2f vn = n * v.dot(n);
		Vector2f vt = v - vn;

		return vt * (1 - friction) + vn * -elasticity;
	}
}
