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
						Vector2f pushVector;
						if (Collision2d::intersectPolygons(p1->edges(), p1->transformation().translation(), p2->edges(), p2->transformation().translation(), pushVector))
						{
							body1->contact(body2);
							body2->contact(body1);

							if (body1->getType() == Body::StaticBody)
							{
								body2->translate(pushVector * -1.0f);
								body2->setVelocity(Vector2f(0, 0));
							}
							else if (body2->getType() == Body::StaticBody)
							{
								body1->translate(pushVector);
								body1->setVelocity(Vector2f(0, 0));
							}
							else
							{
								body1->translate(pushVector * -0.5f);
								body2->translate(pushVector * 0.5f);
							}
						}
					}
				}
			}
		}
	}
}
